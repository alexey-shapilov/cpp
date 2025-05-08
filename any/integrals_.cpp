#include <cmath>
#include <functional>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <iostream>
#include <ostream>
#include <vector>

// Весовая функция w(x)
double weight_function(double x) { return sqrt(x); }

// Подынтегральная функция для вычисления моментов: x^k * w(x)
double moment_integrand(double x, void *params) {
  int k = *static_cast<int *>(params);
  return pow(x, k) * weight_function(x);
}

// Функции для вычисления интегралов
double f0(double x) { return pow(x, 0); } // Константа
double f1(double x) { return x; }         // Линейная
double f2(double x) { return pow(x, 2); } // Квадратичная
double f3(double x) { return pow(x, 2) + pow(x, 3); } // Кубическая
double f5(double x) { return sin(x); } // Неполиномиальная

std::function<double(double)> generate_polynomial(int n) {
  // Генерация коэффициентов для многочлена степени N-1 (1 + 2x + 3x² + ...)
  std::vector<double> coefficients(n);
  for (int i = 0; i < n; ++i) {
    coefficients[i] = i + 1; // Пример: коэффициенты 1, 2, 3, ..., N
  }
  // Лямбда-функция для многочлена степени N-1
  auto f4 = [coefficients](double x) {
    double result = 0.0;
    for (size_t i = 0; i < coefficients.size(); ++i) {
      result += coefficients[i] * pow(x, i);
    }
    return result;
  };

  return f4;
}

std::function<double(double)> select_function(int n) {
  std::cout << "Функции:\n";
  std::cout << "1. Многочлен степени 0\n";
  std::cout << "2. Многочлен степени 1\n";
  std::cout << "3. Многочлен степени 2\n";
  std::cout << "4. Многочлен степени 3\n";
  std::cout << "5. Многочлен степени N-1\n";
  std::cout << "6. sin(x)\n";

  int selected_function;
  std::cout << "Выберите функцию: ";
  std::cin >> selected_function;

  switch (selected_function) {
  case 1:
    return f0;
  case 2:
    return f1;
  case 3:
    return f2;
  case 4:
    return f3;
  case 5:
    return generate_polynomial(n);
  default:
    return f5;
  }
}

int main() {
  // Ввод параметров
  int N;
  double a, b;
  std::cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  std::cin >> a >> b;
  std::cout << "Введите количество узлов N: ";
  std::cin >> N;

  std::function<double(double)> f = select_function(N);

  // Генерация равномерных узлов
  std::vector<double> nodes(N);
  for (int i = 0; i < N; ++i) {
    nodes[i] = a + i * (b - a) / (N - 1);
  }

  // Этап 1: Вычисление коэффициентов ИКФ
  // -------------------------------------
  // 1. Вычисление моментов
  std::vector<double> moments(N);

  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  for (int i = 0; i < N; ++i) {
    gsl_function F;
    F.function = &moment_integrand;
    F.params = &i;
    double result, error;
    gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, workspace, &result, &error);
    moments[i] = result;
  }

  gsl_integration_workspace_free(workspace);

  // 2. Построение матрицы Вандермонда
  gsl_matrix *V = gsl_matrix_alloc(N, N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      gsl_matrix_set(V, i, j, pow(nodes[j], i)); // V[i][j] = nodes[j]^i
    }
  }

  // 3. Решение системы V^T * A = moments
  gsl_vector *m = gsl_vector_alloc(N);
  gsl_vector *A = gsl_vector_alloc(N);
  for (int i = 0; i < N; ++i) {
    gsl_vector_set(m, i, moments[i]);
  }

  // LU-разложение
  gsl_permutation *perm = gsl_permutation_alloc(N);
  int signum;
  gsl_linalg_LU_decomp(V, perm, &signum);
  gsl_linalg_LU_solve(V, perm, m, A);

  // Вывод весов
  std::cout << "Веса ИКФ:\n";
  for (int i = 0; i < N; ++i) {
    std::cout << "A" << i << " = " << gsl_vector_get(A, i) << std::endl;
  }

  // Этап 2: Вычисление интегралов для всех функций
  // ---------------------------------------------
  std::cout << "\nРезультаты интегрирования:\n";

  double result = 0.0;
  for (int i = 0; i < N; ++i) {
    result += gsl_vector_get(A, i) * f(nodes[i]);
  }

  gsl_function F = {[](double x, void *params) -> double {
                      auto &f =
                          *static_cast<std::function<double(double)> *>(params);
                      return f(x);
                    },
                    &f};
  double exact_f, error_exact;
  gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, workspace, &exact_f,
                       &error_exact);

  std::cout << "Ожидаемое значение интеграла: " << exact_f << std::endl;

  // Для f5 вычисляем погрешности
  double abs_error = fabs(result - exact_f);
  double rel_error = abs_error / exact_f * 100.0;
  std::cout << "f: " << result << " | Абс. погрешность: " << abs_error
            << " | Отн. погрешность: " << rel_error << "%";

  std::cout << std::endl;

  // Освобождение ресурсов
  gsl_matrix_free(V);
  gsl_vector_free(m);
  gsl_vector_free(A);
  gsl_permutation_free(perm);

  return 0;
}
