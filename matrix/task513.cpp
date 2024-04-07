#include <iostream>

using namespace std;

int main() {
  int m{}, n{};
  cout << "Input number of rows: ";
  cin >> m;
  cout << "Input number of columns: ";
  cin >> n;
  cout << endl;

  int min{m};
  int matrix[m][n];

  if (min > n) {
    min = n;
  }

  int counter{0};
  int max_counter{min / 2};

  if (min % 2 == 1) {
    max_counter = min / 2 + 1;
  }

  for (int i{0}; i < max_counter; i++) {
    // Заполнение верхнего и нижнего ряда очередной матрицы
    for (int j{i}; j < n - i; j++) {
      matrix[i][j] = i + 1;
      matrix[m - i - 1][j] = i + 1;
    }
    // Заполнение левого и правого столбца очередной матрицы
    for (int j{i + 1}; j < m - i - 1; j++) {
      matrix[j][i] = i + 1;
      matrix[j][n - i - 1] = i + 1;
    }
  }

  for (int i{0}; i < m; i++) {
    for (int j{0}; j < n; j++) {
      cout << matrix[i][j] << "\t";
    }
    cout << endl;
  }

  return 0;
}
