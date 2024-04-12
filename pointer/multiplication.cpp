#include <cstdlib>
#include <iostream>
#include <ostream>
#define RANDOM_MIN 1
#define RANDOM_MAX 10

int main() {
  int l, m, n;

  std::cout << "Input dimension (rows x columns) for matrix A:" << std::endl
            << "rows: ";
  std::cin >> l;
  std::cout << "columns: ";
  std::cin >> m;

  std::cout << std::endl
            << "Input dimension (rows x columns) for matrix B:" << std::endl
            << "rows: " << m << std::endl
            << "columns: ";
  std::cin >> n;

  int **matrix_a = new int *[l];              // l x m
  int **matrix_b = new int *[m];              // m x n
  int **matrix_multiplication = new int *[l]; // l x n

  std::cout << "Matrix A:" << std::endl;
  for (int i{0}; i < l; i++) {
    matrix_a[i] = new int[m];
    for (int j{0}; j < m; j++) {
      matrix_a[i][j] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
      std::cout << matrix_a[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "Matrix B:" << std::endl;
  for (int i{0}; i < m; i++) {
    matrix_b[i] = new int[n];
    for (int j{0}; j < n; j++) {
      matrix_b[i][j] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
      std::cout << matrix_b[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  for (int i{0}; i < l; i++) {
    matrix_multiplication[i] = new int[n];
  }

  std::cout << std::endl << "Matrix multiplication:\n";
  for (int i{0}; i < l; i++) {
    for (int j{0}; j < n; j++) {
      for (int k{0}; k < m; k++) {
        matrix_multiplication[i][j] += matrix_a[i][k] * matrix_b[k][j];
      }
      std::cout << matrix_multiplication[i][j] << "\t";
    }
    std::cout << std::endl;
  }
  return 0;
}
