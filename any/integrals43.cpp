#include <cmath>
#include <functional>
#include <iomanip>
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

// Уточнение по Рунге-Ромбергу
double runge_romberg(double I_h, double I_hl, int l, int r) {
  return (pow(l, r) * I_hl - I_h) / (pow(l, r) - 1);
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

double *calculate_integrals(function<double(double)> f, double a, double b,
                            int m) {
  double *I = new double[5];
  I[0] = left_rectangle(f, a, b, m);
  I[1] = right_rectangle(f, a, b, m);
  I[2] = mid_rectangle(f, a, b, m);
  I[3] = trapezoid(f, a, b, m);
  I[4] = simpson(f, a, b, m);

  return I;
}

int main() {
  setlocale(LC_ALL, "Russian");

  do {
    // Ввод параметров
    double a, b;
    cout << "Введите концы промежутка интегрирования (a, b) через пробел: ";
    cin >> a >> b;
    int m;
    cout << "Введите количество промежутков деления: ";
    cin >> m;

    function<double(double)> f = select_function();

    double exact = primitives[selected_function - 1](b) -
                   primitives[selected_function - 1](a);

    double *I = calculate_integrals(f, a, b, m);

    std::cout << "Точное значение: " << exact
              << "\nЛевые прямоугольники: " << I[0]
              << " (Δ = " << fabs(I[0] - exact) << ")"
              << "\nПравые прямоугольники: " << I[1]
              << " (Δ = " << fabs(I[1] - exact) << ")"
              << "\nСредние прямоугольники: " << I[2]
              << " (Δ = " << fabs(I[2] - exact) << ")" << "\nТрапеции: " << I[3]
              << " (Δ = " << fabs(I[3] - exact) << ")" << "\nСимпсон: " << I[4]
              << " (Δ = " << fabs(I[4] - exact) << ")\n\n";

    int c;
    std::cout << "1. Уточнить вычесления по Рунге-Ромбергу\n"
              << "0. Назад\n";
    std::cin >> c;

    double l;
    switch (c) {
    case 1: {
      std::cout << "Введите во сколько раз увеличить количество разбиений: ";
      std::cin >> l;
      double *I_hl = calculate_integrals(f, a, b, m * l);

      cout << endl;
      // Порядки точности для методов
      vector<pair<int, string>> methods = {{1, "Левые прямоугольники"},
                                           {1, "Правые прямоугольники"},
                                           {2, "Средние прямоугольники"},
                                           {2, "Трапеции"},
                                           {4, "Симпсон"}};
      cout << setw(25) << "СКФ" << setw(15) << "J(h)" << setw(15) << "|J-J(h)|"
           << setw(15) << "J(h/l)" << setw(15) << "|J-J(h/l)|" << setw(15)
           << "Jp" << setw(15) << "|j-Jp|" << endl;
      for (int i{0}; i < methods.size(); i++) {
        double refined = runge_romberg(I[i], I_hl[i], l, methods[i].first);
        cout << setw(25) << methods[i].second << setw(15) << I[i] << setw(15)
             << fabs(exact - I[i]) << setw(15) << I_hl[i] << setw(15)
             << fabs(exact - I_hl[i]) << setw(15) << refined << setw(15)
             << fabs(exact - refined) << endl;
      }
      cout << endl;
      break;
    }
    }
  } while (true);

  return 0;
}
