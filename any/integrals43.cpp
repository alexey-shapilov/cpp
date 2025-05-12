#include <cmath>
#include <functional>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <iostream>
#include <ostream>

using namespace std;

double left_rectangle(function<double(double)> f, double a, double b, int m) {
  double h = (b - a) / m;
  double sum = 0.0;
  for (int i = 0; i < m; i++)
    sum += f(a + i * h);
  return sum * h;
}

double right_rectangle(function<double(double)> f, double a, double b, int m) {
  double h = (b - a) / m;
  double sum = 0.0;
  for (int i = 1; i <= m; i++)
    sum += f(a + i * h);
  return sum * h;
}

double mid_rectangle(function<double(double)> f, double a, double b, int m) {
  double h = (b - a) / m;
  double sum = 0.0;
  for (int i = 0; i < m; i++)
    sum += f(a + (i + 0.5) * h);
  return sum * h;
}

double trapezoid(function<double(double)> f, double a, double b, int m) {
  double h = (b - a) / m;
  double sum = 0.5 * (f(a) + f(b));
  for (int i = 1; i < m; i++)
    sum += f(a + i * h);
  return sum * h;
}

double simpson(function<double(double)> f, double a, double b, int m) {
  double h = (b - a) / m;
  double sum = f(a) + f(b);
  for (int i = 1; i < m; i++) {
    double x = a + i * h;
    sum += 2 * f(a + i * h) + 4 * f(a + 0.5 * h);
  }
  return sum * h / 6.0;
}

double simpson1(function<double(double)> f, double a, double b, int m) {
  if (m % 2 != 0)
    m++;
  double h = (b - a) / m;
  double sum = f(a) + f(b);
  for (int i = 1; i < m; i++) {
    double x = a + i * h;
    sum += (i % 2 == 0) ? 2 * f(x) : 4 * f(x);
  }
  return sum * h / 3.0;
}

// Функции для вычисления интегралов
double f0(double x) { return 1.0; }           // Константа
double f1(double x) { return x; }             // Линейная
double f2(double x) { return x + pow(x, 2); } // Квадратичная
double f3(double x) { return x + pow(x, 2) + pow(x, 3); } // Кубическая
double f4(double x) { return sin(x); } // Неполиномиальная

function<double(double)> select_function() {
  cout << "Функции:\n";
  cout << "1. Многочлен степени 0\n";
  cout << "2. Многочлен степени 1\n";
  cout << "3. Многочлен степени 2\n";
  cout << "4. Многочлен степени 3\n";
  cout << "5. sin(x)\n\n";

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
  default:
    cout << "Выбрана функция 5\n\n";
    return f4;
  }
}

int main() {
  // Ввод параметров
  double a, b;
  cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  cin >> a >> b;
  int m;
  cout << "Введите количество промежутков деления: ";
  cin >> m;

  function<double(double)> f = select_function();

  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  gsl_function F = {[](double x, void *params) -> double {
                      auto &f =
                          *static_cast<function<double(double)> *>(params);

                      return f(x);
                    },
                    &f};
  double exact, error_exact;
  gsl_integration_qags(&F, a, b, 0, 1e-7, 1000, workspace, &exact,
                       &error_exact);
  gsl_integration_workspace_free(workspace);

  double I;

  std::cout << "\nТочное значение: " << exact
            << "\nЛевый прямоугольник: " << (I = left_rectangle(f, a, b, m))
            << " (Δ = " << fabs(I - exact) << ")"
            << "\nПравый прямоугольник: " << (I = right_rectangle(f, a, b, m))
            << " (Δ = " << fabs(I - exact) << ")"
            << "\nСредний прямоугольник: " << (I = mid_rectangle(f, a, b, m))
            << " (Δ = " << fabs(I - exact) << ")"
            << "\nТрапеция: " << (I = trapezoid(f, a, b, m))
            << " (Δ = " << fabs(I - exact) << ")"
            << "\nСимпсон: " << (I = simpson1(f, a, b, m))
            << " (Δ = " << fabs(I - exact) << ")\n";
  return 0;
}
