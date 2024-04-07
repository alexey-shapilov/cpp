#include <cstdlib>
#include <iostream>
const int kArraySize = 8;
const int kMinRandom = 1;
const int kMaxRandom = 500;

void BubleSortAsc(int *arr, int array_size) {
  int temp;
  int *iterator = arr;

  for (int i{0}; i < array_size; i++) {
    iterator = arr;
    // В результате итерации по элементам массива очередной, самый большой
    // элемент будет в самом конце массива, поэтому, каждый следующий проход по
    // элементам массива выполняется до элемента, перемещенного в конец массива
    // на предыдущем проходе.
    //
    // Для этого от длинны массива отнимается номер текущего прохода i
    for (int j{0}; j < array_size - i - 1; j++) {
      if (*iterator > *(iterator + 1)) {
        temp = *iterator;
        *iterator = *(iterator + 1);
        *(iterator + 1) = temp;
      }
      iterator++;
    }
  }
}

void PrintArray(int *arr, int array_size) {
  for (int i{0}; i < kArraySize; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  int arr[kArraySize];

  for (int i{0}; i < kArraySize; i++) {
    arr[i] = rand() % (kMaxRandom - kMinRandom + 1) + kMinRandom;
  }

  PrintArray(arr, kArraySize);

  BubleSortAsc(arr, kArraySize);

  PrintArray(arr, kArraySize);

  return 0;
}
