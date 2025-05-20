#include <cmath>
#include <functional>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector_double.h>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>

#define EPSILON 1e-12

using namespace std;

struct Interval {
  double a;
  double b;
};

struct Params {
  Interval interval;
  int n;
  vector<double> nodes;
};

// Весовая функция w(x)
double weight_function(double x) { return exp(x); }

// Подынтегральная функция для вычисления моментов: x^k * w(x)
double moment_integrand(double x, void *params) {
  int k = *static_cast<int *>(params);
  return pow(x, k) * weight_function(x);
}

double f2(double x) { return sin(x); } // Неполиномиальная

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

int selected_function;
function<double(double)> select_function(int n) {
  cout << "Функции:\n";
  cout << "1. Многочлен степени N-1\n";
  cout << "2. sin(x)\n\n";

  cout << "Выберите функцию: ";
  cin >> selected_function;

  switch (selected_function) {
  case 1:
    cout << "Выбрана функция 1\n\n";
    return generate_polynomial(n);
  }
  cout << "Выбрана функция 2\n\n";
  return f2;
}

Params input_params() {
  Interval interval;
  Params params;

  cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  cin >> interval.a >> interval.b;

  cout << "Введите количество узлов N: ";
  cin >> params.n;

  params.interval = interval;

  return params;
}

gsl_vector *calculate_coefficients(Interval interval, double n) {
  int N = 2 * n;

  // Вычисление моментов
  vector<double> moments(N);

  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  for (int i = 0; i < N; ++i) {
    gsl_function F;
    F.function = &moment_integrand;
    F.params = &i;
    double result, error;
    gsl_integration_qags(&F, interval.a, interval.b, 0, 1e-7, 1000, workspace,
                         &result, &error);
    moments[i] = result;
  }

  gsl_matrix *V = gsl_matrix_alloc(n, n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      gsl_matrix_set(V, i, j, moments[j + i]);
    }
  }

  // Решение системы V^T * A = moments
  gsl_vector *m = gsl_vector_alloc(n);
  gsl_vector *A = gsl_vector_alloc(n);
  for (int i = n; i < N; ++i) {
    gsl_vector_set(m, i - n, -moments[i]);
  }

  // LU-разложение
  gsl_permutation *perm = gsl_permutation_alloc(n);
  int signum;
  gsl_linalg_LU_decomp(V, perm, &signum);
  gsl_linalg_LU_solve(V, perm, m, A);

  gsl_matrix_free(V);
  gsl_vector_free(m);
  gsl_permutation_free(perm);
  gsl_integration_workspace_free(workspace);

  return A;
}

gsl_vector *calculate_coefficients_vandermond(Interval interval,
                                              vector<double> nodes) {
  int n = nodes.size();

  // Вычисление моментов
  vector<double> moments(n);

  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  for (int i = 0; i < n; ++i) {
    gsl_function F;
    F.function = &moment_integrand;
    F.params = &i;
    double result, error;
    gsl_integration_qags(&F, interval.a, interval.b, 0, 1e-7, 1000, workspace,
                         &result, &error);
    moments[i] = result;
  }

  gsl_matrix *V = gsl_matrix_alloc(n, n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      gsl_matrix_set(V, i, j, pow(nodes[j], i)); // V[i][j] = nodes[j]^i
    }
  }

  // Решение системы V^T * A = moments
  gsl_vector *m = gsl_vector_alloc(n);
  gsl_vector *A = gsl_vector_alloc(n);
  for (int i = 0; i < n; ++i) {
    gsl_vector_set(m, i, moments[i]);
  }

  // LU-разложение
  gsl_permutation *perm = gsl_permutation_alloc(n);
  int signum;
  gsl_linalg_LU_decomp(V, perm, &signum);
  gsl_linalg_LU_solve(V, perm, m, A);

  gsl_matrix_free(V);
  gsl_vector_free(m);
  gsl_permutation_free(perm);
  gsl_integration_workspace_free(workspace);

  return A;
}

double wn(double x, gsl_vector *A, int n) {
  double result = gsl_vector_get(A, 0);
  for (size_t i = 1; i < n; ++i) {
    result += gsl_vector_get(A, i) * pow(x, i);
  }
  return result + pow(x, n);
}

double bisection(double left, double right, gsl_vector *A, int n,
                 double epsilon) {
  int w = 0;
  double c = left;
  while ((right - left) >= 2 * epsilon) {
    w++;
    c = (left + right) / 2;
    if (wn(c, A, n) == 0.0)
      break;
    else if (wn(c, A, n) * wn(left, A, n) < 0)
      right = c;
    else
      left = c;
  }

  return c;
}

vector<double> find_all_roots(gsl_vector *coeffs, double left, double right,
                              int n, double eps = 1e-6) {
  vector<double> roots;
  double step = (right - left) / n;
  if (step > 0.1) {
    step = 0.1;
  }

  // Рекурсивная проверка интервалов
  for (double a = left; a < right; a += step) {
    double b = a + step;
    double fa = wn(a, coeffs, n);
    double fb = wn(b, coeffs, n);

    // Если есть смена знака, ищем корень
    if (fa * fb < 0) {
      try {
        double root = bisection(a, b, coeffs, n, eps);

        // Проверка на уникальность (избегаем дубликатов)
        bool is_unique = true;
        for (double r : roots) {
          if (abs(r - root) < eps) {
            is_unique = false;
            break;
          }
        }

        if (is_unique) {
          roots.push_back(root);
        }
      } catch (...) {
        // Пропускаем интервалы, где метод не сошелся
      }
    }
  }

  return roots;
}

double integrand(function<double(double)> f, gsl_vector *A,
                 vector<double> nodes) {
  int n = nodes.size();
  double result = 0.0;
  for (int i = 0; i < n; ++i) {
    result += gsl_vector_get(A, i) * f(nodes[i]);
  }

  return result;
}

double exact_integrand(function<double(double)> f, Interval interval) {
  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  gsl_function F = {[](double x, void *params) -> double {
                      auto &f =
                          *static_cast<function<double(double)> *>(params);

                      return f(x) * weight_function(x);
                    },
                    &f};
  double exact, error_exact;
  gsl_integration_qags(&F, interval.a, interval.b, 0, 1e-7, 1000, workspace,
                       &exact, &error_exact);
  gsl_integration_workspace_free(workspace);

  return exact;
}

int main() {
  setlocale(LC_ALL, "Russian");
  cout << fixed << setprecision(12);

  Params params = input_params();

  gsl_vector *A_orthogonal = calculate_coefficients(params.interval, params.n);
  params.nodes = find_all_roots(A_orthogonal, params.interval.a,
                                params.interval.b, params.n);

  gsl_vector *A =
      calculate_coefficients_vandermond(params.interval, params.nodes);

  function<double(double)> f = select_function(2 * params.n - 1);

  int choice;
  do {
    // Вывод весов
    cout << "Коэффициенты ИКФ:\n";
    for (int i = 0; i < params.n; ++i) {
      cout << "x" << i << " = " << params.nodes[i] << " : A" << i << " = "
           << gsl_vector_get(A, i) << endl;
    }

    cout << "\nРезультаты интегрирования:\n";

    double result = integrand(f, A, params.nodes);
    double exact_f = exact_integrand(f, params.interval);

    cout << "Ожидаемое значение интеграла w(x)f(x): " << exact_f << endl;

    double abs_error = fabs(result - exact_f);
    double rel_error = abs_error / exact_f * 100.0;
    cout << "f: " << result << " | Абс. погрешность: " << abs_error
         << " | Отн. погрешность: " << rel_error << "%";

    cout << endl << endl;

    cout << "1. Выбрать новую функцию\n"
         << "2. Ввести новые значения промежутка интегрирования\n"
         << "0. Выход\n";
    cin >> choice;

    if (choice == 1) {
      f = select_function(2 * params.n - 1);
    } else if (choice == 2) {
      gsl_vector_free(A);
      gsl_vector_free(A_orthogonal);

      params = input_params();

      A_orthogonal = calculate_coefficients(params.interval, params.n);
      params.nodes = find_all_roots(A_orthogonal, params.interval.a,
                                    params.interval.b, params.n);

      A = calculate_coefficients_vandermond(params.interval, params.nodes);

      if (selected_function == 1) {
        f = generate_polynomial(params.n);
      }
    }
  } while (choice != 0);

  gsl_vector_free(A_orthogonal);
  gsl_vector_free(A);

  return 0;
}
