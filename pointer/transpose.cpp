#include <cstdlib>
#include <iostream>
#define RANDOM_MIN 1
#define RANDOM_MAX 1000

int main() {
  int m, n;

  std::cout << "Input dimension for matrix:" << std::endl << "rows: ";
  std::cin >> m;
  std::cout << "columns: ";
  std::cin >> n;

  int **matrix = new int *[m];
  int **matrix_transpose = new int *[n];

  for (int i{0}; i < m; i++) {
    matrix[i] = new int[n];
  }
  for (int i{0}; i < n; i++) {
    matrix_transpose[i] = new int[m];
  }

  std::cout << "Matrix:\n";
  for (int i{0}; i < m; i++) {
    for (int j{0}; j < n; j++) {
      matrix[i][j] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
      matrix_transpose[j][i] = matrix[i][j];

      std::cout << matrix[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  std::cout << "\nMatrix transpose:\n";
  for (int i{0}; i < n; i++) {
    for (int j{0}; j < m; j++) {
      std::cout << matrix_transpose[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  return 0;
}
