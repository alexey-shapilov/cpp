#include <cmath>
#include <functional>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>

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
double f0(double x) { return 1.0; }           // Константа
double f1(double x) { return x; }             // Линейная
double f2(double x) { return x + pow(x, 2); } // Квадратичная
double f3(double x) { return x + pow(x, 2) + pow(x, 3); } // Кубическая
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

int selected_function;
function<double(double)> select_function(int n) {
  cout << "Функции:\n";
  cout << "1. Многочлен степени 0\n";
  cout << "2. Многочлен степени 1\n";
  cout << "3. Многочлен степени 2\n";
  cout << "4. Многочлен степени 3\n";
  cout << "5. Многочлен степени N-1\n";
  cout << "6. sin(x)\n\n";

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

Params input_params() {
  Interval interval;
  Params params;

  cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  cin >> interval.a >> interval.b;

  cout << "Введите количество узлов N: ";
  cin >> params.n;

  vector<double> nodes(params.n);
  cout << "Введите узлы: ";
  for (int i = 0; i < params.n; ++i) {
    cin >> nodes[i];
  }
  // Генерация равномерных узлов
  // for (int i = 0; i < params.n; ++i) {
  //  nodes[i] = interval.a + i * (interval.b - interval.a) / (params.n - 1);
  //}

  params.interval = interval;
  params.nodes = nodes;

  return params;
}

gsl_vector *calculate_coefficientsi_vandermond(Interval interval,
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

gsl_vector *calculate_coefficientsi_integrand(Interval interval,
                                              vector<double> nodes) {
  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  int n = nodes.size();
  gsl_vector *A = gsl_vector_alloc(n);

  for (int i = 0; i < n; ++i) {
    struct lkn_params lknParams = {nodes, i};
    gsl_function F;
    F.function = &coeff_integrand;
    F.params = &lknParams;
    double r, e;
    gsl_integration_qags(&F, interval.a, interval.b, 0, 1e-7, 1000, workspace,
                         &r, &e);

    gsl_vector_set(A, i, r);
  }
  gsl_integration_workspace_free(workspace);

  return A;
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

  gsl_vector *A =
      calculate_coefficientsi_vandermond(params.interval, params.nodes);
  gsl_vector *A_integrand =
      calculate_coefficientsi_integrand(params.interval, params.nodes);

  function<double(double)> f = select_function(params.n);

  int choice;
  do {
    // Вывод весов
    cout << "Коэффициенты ИКФ:\n";
    for (int i = 0; i < params.n; ++i) {
      cout << "x" << i << " = " << params.nodes[i] << " : A" << i << " = "
           << gsl_vector_get(A, i) << " ~ " << gsl_vector_get(A_integrand, i)
           << endl;
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
      f = select_function(params.n);
    } else if (choice == 2) {
      // Освобождение ресурсов
      gsl_vector_free(A);
      gsl_vector_free(A_integrand);

      params = input_params();
      if (selected_function == 5) {
        f = generate_polynomial(params.n);
      }
      A = calculate_coefficientsi_vandermond(params.interval, params.nodes);
      A_integrand =
          calculate_coefficientsi_integrand(params.interval, params.nodes);
    }
  } while (choice != 0);

  return 0;
}
