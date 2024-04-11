#include <cstdlib>
#include <iostream>
#define MATRIX_ROWS 3
#define MATRIX_COLUMNS 5
#define RANDOM_MIN 1
#define RANDOM_MAX 1000

int main() {
  int matrix[MATRIX_ROWS][MATRIX_COLUMNS];
  int **matrix_transpose = new int *[MATRIX_COLUMNS];

  for (int i{0}; i < MATRIX_COLUMNS; i++) {
    matrix_transpose[i] = new int[MATRIX_ROWS];
  }

  std::cout << "Matrix:\n";
  for (int i{0}; i < MATRIX_ROWS; i++) {
    for (int j{0}; j < MATRIX_COLUMNS; j++) {
      matrix[i][j] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
      matrix_transpose[j][i] = matrix[i][j];

      std::cout << matrix[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  std::cout << "\nMatrix transpose:\n";
  for (int i{0}; i < MATRIX_COLUMNS; i++) {
    for (int j{0}; j < MATRIX_ROWS; j++) {
      std::cout << matrix_transpose[i][j] << "\t";
    }
    std::cout << std::endl;
  }

  return 0;
}
