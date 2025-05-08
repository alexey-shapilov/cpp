#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
// объявление функции
double func(double x) { return sin(5 * x) + pow(x, 2) - 1; }
// производная
double derivFunc(double x) { return 5 * cos(5 * x) + 2 * x; }
// Реализация метода Ньютона
double newtonRaphson(double x, double EPSILON) {
  int w = 0;
  double h1 = func(x) / derivFunc(x);
  while (abs(h1) >= EPSILON) {
    w++;
    h1 = func(x) / derivFunc(x);
    x = x - h1;
  }
  cout << "Number of iterations: " << w << endl;
  cout << "Residual modulus: " << abs(func(x) - 0) << endl;
  return x;
}
// Реализация улучшенного метода Ньютона
double newtonRaphsonimprove(double x0, double EPSILON) {
  double h2 = func(x0) / derivFunc(x0);
  double x = x0 - h2;
  int w = 0;
  while (abs(h2) >= EPSILON) {
    w++;
    h2 = func(x) / derivFunc(x0);
    x = x - h2;
  }
  cout << "Number of iterations: " << w << endl;
  cout << "Residual modulus: " << abs(func(x) - 0) << endl;
  return x;
}
// Реализация метода секущих
double secantMethod(double x0, double x1, double EPSILON) {
  int w = 0;
  double f0 = func(x0);
  double f1 = func(x1);
  double h3 = f1 * (x1 - x0) / (f1 - f0);
  double xkk = x1;
  double xk = x1 - h3;
  double tmp;
  while (abs(h3) >= EPSILON) {
    w++;
    h3 = func(xk) * (xk - xkk) / (func(xk) - func(xkk));
    tmp = xk;
    xk = xk - h3;
    xkk = tmp;
  }
  cout << "Number of iterations: " << w << endl;
  cout << "Residual modulus: " << abs(func(xk) - 0) << endl;
  return xk;
}
// Реализация метода биссекции
double bisection(double left, double right, double EPSILON) {
  int w = 0;
  double c = left;
  while ((right - left) >= 2 * EPSILON) {
    w++;
    c = (left + right) / 2;
    if (func(c) == 0.0)
      break;
    else if (func(c) * func(left) < 0)
      right = c;
    else
      left = c;
  }
  cout << "Number of iterations: " << w << endl;
  cout << "Residual modulus: " << abs(func(c) - 0) << endl;
  return c;
}

int main() {
  char result_choice;
  do {
    // Ввод значения погрешности
    double EPSILON;
    cout << "Put EPSILON: ";
    cin >> EPSILON;
    // Ввод шага
    double N;
    cout << "Put N: ";
    cin >> N;
    // Ввод концов отрезка
    double A, B;
    cout << "Put A: ";
    cin >> A;
    cout << "Put B: ";
    cin >> B;
    // Расчет шага
    double h = (B - A) / N;
    vector<vector<double>> segments;
    double f1, f2;
    // Расчет отрезков
    cout << "Segments:\n";
    for (double a = A; a < B; a += h) {
      f1 = func(a);
      f2 = func(a + h);
      if ((f1 * f2) < 0) {
        segments.push_back({a, a + h});
        cout << segments.size() << ". [" << a << ", " << a + h << "]\n";
      }
    }
    // Проверка отрезка
    if (segments.empty()) {
      cout << "No root segments found. Exiting.\n";
      return 0;
    }
    // Реализация решения нелинейного уравнения
    char choice;
    do {
      int segment_choice;
      cout << "Choose a segment number: ";
      cin >> segment_choice;
      if (segment_choice < 1 || segment_choice > segments.size()) {
        cout << "Invalid segment choice. Try again.\n";
        continue;
      }

      double left = segments[segment_choice - 1][0];
      double right = segments[segment_choice - 1][1];

      char method_choice;
      do {
        cout << "Choose method:\n";
        cout << "1. Newton-Raphson\n";
        cout << "2. Improved Newton-Raphson\n";
        cout << "3. Secant Method\n";
        cout << "4. Bisection Method\n";

        int method;
        cin >> method;

        double root;
        switch (method) {
        case 1:
          root = newtonRaphson((left + right) / 2, EPSILON);
          break;
        case 2:
          root = newtonRaphsonimprove((left + right) / 2, EPSILON);
          break;
        case 3:
          root = secantMethod(left, right, EPSILON);
          break;
        case 4:
          root = bisection(left, right, EPSILON);
          break;
        default:
          cout << "Invalid method choice. Try again.\n";
          continue;
        }

        cout << "Approximate root: " << root << endl;
        cout << "Do you want to change the method for this segment? (y/n): ";
        cin >> method_choice;
      } while (method_choice == 'y' || method_choice == 'Y');

      cout << "Do you want to choose another segment? (y/n): ";
      cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Do you want to restart (y/n): ";
    cin >> result_choice;
  } while (result_choice == 'y' || result_choice == 'Y');

  return 0;
}
