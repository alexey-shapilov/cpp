#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

// Пример функции f(x),
double f(double x) {
  return sqrt(1 + x) + x; //
}

// Интерполяционный многочлен в форме Лагранжа
double lagrangeInterpolation(const vector<double> &x, const vector<double> &y,
                             double x_interp) {
  int n = x.size();
  double result = 0;
  for (int i = 0; i < n; i++) {
    double term = y[i];
    for (int k = 0; k < n; k++) {
      if (k != i) {
        term *= (x_interp - x[k]) / (x[i] - x[k]);
      }
    }
    result += term;
  }
  return result;
}

// Интерполяция в форме Ньютона
vector<vector<double>> dividedDifferencesTable(const vector<double> &x,
                                               const vector<double> &y) {
  int n = x.size();
  vector<vector<double>> dd(n, vector<double>(n));
  for (int i = 0; i < n; i++) {
    dd[i][0] = y[i];
  }
  for (int j = 1; j < n; j++) {
    for (int i = 0; i < n - j - 1; i++) {
      dd[i][j] = (dd[i + 1][j - 1] - dd[i][j - 1]) / (x[i + j] - x[i]);
    }
  }
  return dd;
}

vector<double> getNewtonCoefficients(const vector<vector<double>> &dd) {
  int n = dd.size();
  vector<double> coeffs(n);
  for (int i = 0; i < n; i++) {
    coeffs[i] = dd[0][i];
  }
  return coeffs;
}

double newtonInterpolation(const vector<double> &x,
                           const vector<double> &coeffs, double x_interp) {
  double result = coeffs[0];
  double term = 1;
  for (int i = 1; i < x.size(); i++) {
    term *= x_interp - x[i - 1];
    result += coeffs[i] * term;
  }
  return result;
}

void printDividedDifferencesTable(const vector<double> &x,
                                  const vector<vector<double>> &dd) {
  cout << "\nТаблица разделенных разностей:\n";
  for (int j = 0; j < dd[0].size(); j++) {
    cout << "Порядок " << j << ":\n";
    for (int i = 0; i < dd.size() - j; i++) {
      cout << "Узлы: ";
      for (int k = i; k <= i + j; k++) {
        cout << x[k] << " ";
      }
      cout << "-> Разность: " << setw(12) << dd[i][j] << endl;
    }
    cout << endl;
  }
}

template <typename T> void printRow(const vector<T> &data) {
  for (int i = 0; i < data.size(); i++) {
    cout << data[i] << "\t|";
  }
  cout << endl;
}

template <typename T>
void printRowWitName(const string name, const vector<T> &data) {
  cout << name << "\t|";
  printRow(data);
}

int main() {
  setlocale(LC_ALL, "Russian");
  int m;
  cout << "Введите число значений в таблице (m+1): ";
  cin >> m;

  double a, b;
  cout << "Введите границы интервала [a, b]: ";
  cin >> a >> b;

  vector<double> x(m), y(m);
  double step = (b - a) / m;
  for (int i = 0; i < m; i++) {
    x[i] = a + i * step;
    y[i] = f(x[i]);
  }

  cout << "Исходная таблица значений:\n";
  printRowWitName("x", x);
  printRowWitName("y", y);

  double x_interp;
  cout << "Введите точку интерполирования x: ";
  cin >> x_interp;

  int n;
  do {
    cout << "Введите степень интерполяционного многочлена (n <= m-1): ";
    cin >> n;
    if (n > m - 1) {
      cout << "Введено недопустимое значение n. Повторите ввод.\n";
    }
  } while (n > m - 1);

  // Отбираем (n+1) ближайших узлов
  vector<pair<double, pair<double, double>>> nodes;
  for (int i = 0; i < m; i++) {
    nodes.push_back({fabs(x[i] - x_interp), {x[i], y[i]}});
  }
  sort(nodes.begin(), nodes.end());

  vector<double> x_selected, y_selected;
  for (int i = 0; i <= n + 1; i++) {
    x_selected.push_back(nodes[i].second.first);
    y_selected.push_back(nodes[i].second.second);
  }

  cout << "\nОтсортированные узлы интерполяции:\n";
  for (int i = 0; i <= n + 1; i++) {
    cout << "x[" << i << "] = " << x_selected[i] << ", f(x) = " << y_selected[i]
         << ", distance = " << nodes[i].first << endl;
  }

  double Pn_L = lagrangeInterpolation(x_selected, y_selected, x_interp);
  vector<vector<double>> dd = dividedDifferencesTable(x_selected, y_selected);
  vector<double> newton_coeffs = getNewtonCoefficients(dd);
  double Pn_N = newtonInterpolation(x_selected, newton_coeffs, x_interp);
  double error_L = fabs(f(x_interp) - Pn_L);
  double error_N = fabs(f(x_interp) - Pn_N);

  printDividedDifferencesTable(x_selected, dd);

  cout << "\nИнтерполяция Лагранжа: Pn_L(x) = " << Pn_L
       << ", модуль невязки = " << error_L << endl;
  cout << "Интерполяция Ньютона: Pn_N(x) = " << Pn_N
       << ", модуль невязки = " << error_N << endl;

  return 0;
}
