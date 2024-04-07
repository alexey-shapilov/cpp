#include <cstdlib>
#include <iostream>

const int kArraySize = 8;
const int kMinRandom = 1;
const int kMaxRandom = 500;

void InsertSortAsc(int *arr, int array_size) {
  int temp;

  for (int i{1}; i < array_size; i++) {
    for (int j{i}; j > 0 && *(arr + j - 1) > *(arr + j); j--) {
      temp = *(arr + j - 1);
      *(arr + j - 1) = *(arr + j);
      *(arr + j) = temp;
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

  InsertSortAsc(arr, kArraySize);

  PrintArray(arr, kArraySize);

  return 0;
}
