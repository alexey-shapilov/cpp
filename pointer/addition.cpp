#include <cstdlib>
#include <iostream>
#include <ostream>
#define RANDOM_MIN 1
#define RANDOM_MAX 10

int main() {
  int m, n;

  std::cout << "Input dimension (rows x columns) for matrix A and B:"
            << std::endl
            << "rows: ";
  std::cin >> m;
  std::cout << "columns: ";
  std::cin >> n;

  int **matrix_a = new int *[m];
  int **matrix_b = new int *[m];
  int **matrix_addition = new int *[m];

  std::cout << "Matrix A:" << std::endl;
  for (int i{0}; i < m; i++) {
    matrix_a[i] = new int[n];
    matrix_b[i] = new int[n];
    matrix_addition[i] = new int[n];
    for (int j{0}; j < n; j++) {
      matrix_a[i][j] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
      matrix_b[i][j] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
      matrix_addition[i][j] = matrix_a[i][j] + matrix_b[i][j];
      std::cout << matrix_a[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "Matrix B:" << std::endl;
  for (int i{0}; i < m; i++) {
    for (int j{0}; j < n; j++) {
      std::cout << matrix_b[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "Matrix addition:\n";
  for (int i{0}; i < m; i++) {
    for (int j{0}; j < n; j++) {
      std::cout << matrix_addition[i][j] << "\t";
    }
    std::cout << std::endl;
  }
  return 0;
}
