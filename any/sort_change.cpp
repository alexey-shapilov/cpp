#include <cstdlib>
#include <iostream>

#define ODD true
#define EVEN false

const int kArraySize = 9;
const int kMinRandom = 1;
const int kMaxRandom = 500;

bool PairedComparisionAsc(int *arr, bool odd, int array_size) {
  int temp;
  bool exchanged{false};

  int i{odd ? 1 : 0};
  arr += i;
  for (; i < array_size; i += 2) {
    if (i + 1 < array_size && *arr > *(arr + 1)) {
      temp = *arr;
      *arr = *(arr + 1);
      *(arr + 1) = temp;
      exchanged = true;
    }
    if (i + 2 < array_size) {
      arr += 2;
    }
  }

  return exchanged;
}

// Четно-нечетная сортировка
void OddEvenSort(int *arr, int array_size) {
  bool is_sorted{true};

  while (is_sorted) {
    is_sorted = PairedComparisionAsc(arr, EVEN, array_size);
    is_sorted = PairedComparisionAsc(arr, ODD, array_size);
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

  OddEvenSort(arr, kArraySize);

  PrintArray(arr, kArraySize);

  return 0;
}
