#include <cmath>
#include <functional>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <iostream>
#include <vector>

// Весовая функция w(x) = e^x
double weight_function(double x) { return exp(x); }

// Подынтегральная функция для вычисления моментов: x^k * w(x)
double moment_integrand(double x, void *params) {
  int k = *static_cast<int *>(params);
  return pow(x, k) * weight_function(x);
}

// Функции для вычисления интегралов
double f0(double x) { return 1.0; }       // Константа
double f1(double x) { return x; }         // Линейная
double f2(double x) { return x * x; }     // Квадратичная
double f3(double x) { return x * x * x; } // Кубическая
double f5(double x) { return sin(x); }    // Неполиномиальная

// Точное значение интеграла для f5(x) = sin(x) на [0, 1] с весом e^x
double exact_f5() {
  const double e = exp(1.0);
  return 0.5 * (e * (sin(1.0) - cos(1.0))) + 0.5;
}

int main() {
  // Ввод параметров
  int N;
  double a, b;
  std::cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  std::cin >> a >> b;
  std::cout << "Введите количество узлов N: ";
  std::cin >> N;

  // Генерация равномерных узлов
  std::vector<double> nodes(N);
  for (int i = 0; i < N; ++i) {
    nodes[i] = a + i * (b - a) / (N - 1);
  }

  // Генерация коэффициентов для многочлена степени N-1 (1 + 2x + 3x² + ...)
  std::vector<double> coefficients(N);
  for (int i = 0; i < N; ++i) {
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

  // Массив функций с поддержкой лямбд
  std::function<double(double)> functions[6] = {f0, f1, f2, f3, f4, f5};

  // Этап 1: Вычисление коэффициентов ИКФ
  // -------------------------------------
  // 1. Вычисление моментов
  double moments[N];

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

  const double exact_f5_val = exact_f5(); // Точное значение для f5

  // Вычисление точного значения для f4 (используем моменты)
  double exact_f4_val = 0.0;
  for (int i = 0; i < N; ++i) {
    exact_f4_val += coefficients[i] * moments[i];
  }

  for (int func_idx = 0; func_idx < 6; ++func_idx) {
    double result = 0.0;
    for (int i = 0; i < N; ++i) {
      result += gsl_vector_get(A, i) * functions[func_idx](nodes[i]);
    }
    std::cout << "f" << func_idx << ": " << result;

    // Проверка точности для f4 (многочлен степени N-1)
    if (func_idx == 4) {
      double abs_error = fabs(result - exact_f4_val);
      double rel_error = abs_error / exact_f4_val * 100.0;
      std::cout << " | Абс. погрешность: " << abs_error
                << " | Отн. погрешность: " << rel_error << "%";
    }

    // Для f5 вычисляем погрешности
    if (func_idx == 5) {
      double abs_error = fabs(result - exact_f5_val);
      double rel_error = abs_error / exact_f5_val * 100.0;
      std::cout << " | Абс. погрешность: " << abs_error
                << " | Отн. погрешность: " << rel_error << "%";
    }

    std::cout << std::endl;
  }

  // Освобождение ресурсов
  gsl_matrix_free(V);
  gsl_vector_free(m);
  gsl_vector_free(A);
  gsl_permutation_free(perm);

  return 0;
}
