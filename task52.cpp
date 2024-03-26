#include <iostream>

int main() {
  int m{5}, n{7};
  std::cout << "Input number of rows: ";
  std::cin >> m;
  std::cout << "Input number of columns: ";
  std::cin >> n;
  std::cout << std::endl;

  int **matrix{new int *[m]};

  for (int i = 0; i < m; i++) {
    matrix[i] = new int[n];
  }

  int counter{1}; // текущее значение элемента
  int max_counter{m*n}; // максимальное значение элемента

  for (int i{0}; i < m; i++) {
    // Проходим горизонтально вправо сверху
    for (int j{i}; j < n - i; j++) {
      matrix[i][j] = counter++;
    }
    if (counter > max_counter) {
      break;
    }
    // Проходим вертикально вниз справа
    for (int k{i + 1}; k < m - i; k++) {
      matrix[k][n - i - 1] = counter++;
    }
    if (counter > max_counter) {
      break;
    }
    // Проходим горизонтально влево снизу
    for (int j{n - i - 2}; j >= i; j--) {
      matrix[m - i - 1][j] = counter++;
    }
    if (counter > max_counter) {
      break;
    }
    // Проходим вертильано вверх слева
    for (int k{m - i - 2}; k > i; k--) {
      matrix[k][i] = counter++;
    }
  }

  for (int i{0}; i < m; i++) {
    for (int j{0}; j < n; j++) {
      std::cout << matrix[i][j] << "\t";
    }
    delete[] matrix[i]; // освобождаем память выделенную для каждого столбца матрицы
    std::cout << std::endl;
  }
  delete[] matrix; // освобождаем память выделенную для строк матрицы
 
  return 0;
}
