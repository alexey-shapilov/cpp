#include <cmath>
#include <functional>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

// Весовая функция w(x)
double weight_function(double x) { return exp(x); }

// Подынтегральная функция для вычисления моментов: x^k * w(x)
double moment_integrand(double x, void *params) {
  int k = *static_cast<int *>(params);
  return pow(x, k) * weight_function(x);
}

double lkn(const vector<double> &nodes, int k, double x) {
  int n = nodes.size();
  double result = 1;
  for (int i = 0; i < n; i++) {
    if (i != k) {
      result *= (x - nodes[i]) / (nodes[k] - nodes[i]);
    }
  }

  return result;
}

struct lkn_params {
  vector<double> nodes;
  int k;
};
double coeff_integrand(double x, void *params) {
  struct lkn_params *p = (struct lkn_params *)params;
  vector<double> nodes = (p->nodes);
  int k = (p->k);

  return weight_function(x) * lkn(nodes, k, x);
}

// Функции для вычисления интегралов
double f0(double x) { return pow(x, 0); } // Константа
double f1(double x) { return x; }         // Линейная
double f2(double x) { return pow(x, 2); } // Квадратичная
double f3(double x) { return pow(x, 2) + pow(x, 3); } // Кубическая
double f5(double x) { return sin(x); } // Неполиномиальная

function<double(double)> generate_polynomial(int n) {
  // Генерация коэффициентов для многочлена степени N-1 (1 + 2x + 3x² + ...)
  vector<double> coefficients(n);
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

function<double(double)> select_function(int n) {
  cout << "Функции:\n";
  cout << "1. Многочлен степени 0\n";
  cout << "2. Многочлен степени 1\n";
  cout << "3. Многочлен степени 2\n";
  cout << "4. Многочлен степени 3\n";
  cout << "5. Многочлен степени N-1\n";
  cout << "6. sin(x)\n\n";

  int selected_function;
  cout << "Выберите функцию: ";
  cin >> selected_function;

  switch (selected_function) {
  case 1:
    cout << "Выбрана функция 1\n\n";
    return f0;
  case 2:
    cout << "Выбрана функция 2\n\n";
    return f1;
  case 3:
    cout << "Выбрана функция 3\n\n";
    return f2;
  case 4:
    cout << "Выбрана функция 4\n\n";
    return f3;
  case 5:
    cout << "Выбрана функция 5\n\n";
    return generate_polynomial(n);
  default:
    cout << "Выбрана функция 6\n\n";
    return f5;
  }
}

int main() {
  // Ввод параметров
  int N;
  double a, b;
  cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  cin >> a >> b;
  cout << "Введите количество узлов N: ";
  cin >> N;

  function<double(double)> f = select_function(N);

  // Генерация равномерных узлов
  vector<double> nodes(N);
  for (int i = 0; i < N; ++i) {
    nodes[i] = a + i * (b - a) / (N - 1);
  }

  // Этап 1: Вычисление коэффициентов ИКФ
  // -------------------------------------
  // 1. Вычисление моментов
  vector<double> moments(N);

  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  for (int i = 0; i < N; ++i) {
    gsl_function F;
    F.function = &moment_integrand;
    F.params = &i;
    double result, error;
    gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, workspace, &result, &error);
    moments[i] = result;
  }

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
  cout << "Коэффициенты ИКФ:\n";
  for (int i = 0; i < N; ++i) {
    struct lkn_params params = {nodes, i};
    gsl_function F;
    F.function = &coeff_integrand;
    F.params = &params;
    double r, e;
    gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, workspace, &r, &e);

    cout << "x" << i << " = " << nodes[i] << " : A" << i << " = "
         << gsl_vector_get(A, i) << endl;
  }

  // Этап 2: Вычисление интегралов для всех функций
  // ---------------------------------------------
  cout << "\nРезультаты интегрирования:\n";

  double result = 0.0;
  for (int i = 0; i < N; ++i) {
    result += gsl_vector_get(A, i) * f(nodes[i]);
  }

  gsl_function F = {[](double x, void *params) -> double {
                      auto &f =
                          *static_cast<function<double(double)> *>(params);

                      return f(x) * weight_function(x);
                    },
                    &f};
  double exact_f, error_exact;
  gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, workspace, &exact_f,
                       &error_exact);

  cout << "Ожидаемое значение интеграла w(x)f(x): " << exact_f << endl;

  // Для f5 вычисляем погрешности
  double abs_error = fabs(result - exact_f);
  double rel_error = abs_error / exact_f * 100.0;
  cout << "f: " << result << " | Абс. погрешность: " << abs_error
       << " | Отн. погрешность: " << rel_error << "%";

  cout << endl;

  // Освобождение ресурсов
  gsl_matrix_free(V);
  gsl_vector_free(m);
  gsl_vector_free(A);
  gsl_permutation_free(perm);
  gsl_integration_workspace_free(workspace);

  return 0;
}
