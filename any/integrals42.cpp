#include <cmath>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>

using namespace std;

struct Interval {
  double a;
  double b;
};

double left_rectangle(function<double(double)> f, Interval interval) {
  return (interval.b - interval.a) * f(interval.a);
}

double right_rectangle(function<double(double)> f, Interval interval) {
  return (interval.b - interval.a) * f(interval.b);
}

double mid_rectangle(function<double(double)> f, Interval interval) {
  return (interval.b - interval.a) * f((interval.a + interval.b) / 2);
}

double trapezoid(function<double(double)> f, Interval interval) {
  return (interval.b - interval.a) / 2 * (f(interval.a) + f(interval.b));
}

double simpson(function<double(double)> f, Interval interval) {
  return (interval.b - interval.a) / 6 *
         (f(interval.a) + 4 * f((interval.a + interval.b) / 2) + f(interval.b));
}

// Функции для вычисления интегралов
double f0(double x) { return 1.0; } // Константа
double f0_primitive(double x) { return x; }

double f1(double x) { return x; } // Линейная
double f1_primitive(double x) { return pow(x, 2) / 2; }

double f2(double x) { return x + pow(x, 2); } // Квадратичная
double f2_primitive(double x) { return pow(x, 2) / 2 + pow(x, 3) / 3; }

double f3(double x) { return x + pow(x, 2) + pow(x, 3); } // Кубическая
double f3_primitive(double x) {
  return pow(x, 2) / 2 + pow(x, 3) / 3 + pow(x, 4) / 4;
}

double f4(double x) { return sin(x); } // Неполиномиальная
double f4_primitive(double x) { return -cos(x); }

function<double(double)> primitives[5] = {
    f0_primitive, f1_primitive, f2_primitive, f3_primitive, f4_primitive};

int selected_function;
function<double(double)> select_function() {
  cout << "Функции:\n";
  cout << "1. Многочлен степени 0\n";
  cout << "2. Многочлен степени 1\n";
  cout << "3. Многочлен степени 2\n";
  cout << "4. Многочлен степени 3\n";
  cout << "5. sin(x)\n\n";

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

Interval input_iterval() {
  Interval interval;

  cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
  cin >> interval.a >> interval.b;

  return interval;
}

struct SelectedFunction {
  function<double(double)> f;
  double exact(Interval interval) {
    return primitives[selected_function - 1](interval.b) -
           primitives[selected_function - 1](interval.a);
  }
};

int main() {
  setlocale(LC_ALL, "Russian");
  cout << fixed << setprecision(8);

  Interval interval = input_iterval();

  SelectedFunction f;
  f.f = select_function();

  double exact;

  int choice;
  do {
    exact = f.exact(interval);

    double I;

    cout << "\nТочное значение: " << exact
         << "\nЛевый прямоугольник: " << (I = left_rectangle(f.f, interval))
         << " (Ошибка = " << fabs(I - exact) << ")"
         << "\nПравый прямоугольник: " << (I = right_rectangle(f.f, interval))
         << " (Ошибка = " << fabs(I - exact) << ")"
         << "\nСредний прямоугольник: " << (I = mid_rectangle(f.f, interval))
         << " (Ошибка = " << fabs(I - exact) << ")"
         << "\nТрапеция: " << (I = trapezoid(f.f, interval))
         << " (Ошибка = " << fabs(I - exact) << ")"
         << "\nСимпсон: " << (I = simpson(f.f, interval))
         << " (Ошибка = " << fabs(I - exact) << ")\n";
    cout << endl;

    cout << "1. Выбрать новую функцию\n"
         << "2. Ввести новые значения промежутка интегрирования\n"
         << "0. Выход\n";
    cin >> choice;

    if (choice == 1) {
      f.f = select_function();
    } else if (choice == 2) {
      interval = input_iterval();
    }
  } while (choice != 0);

  return 0;
}
