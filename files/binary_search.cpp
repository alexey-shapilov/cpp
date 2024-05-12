#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>

using namespace std;

#define MAX_RANDOM 1000000
#define MIN_RANDOM 1

void PrintArray(const int *arr, const int array_size) {
  for (int i{0}; i < array_size; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

int Compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

bool BinarySearch(const int needle, const int *haystack, const int array_size) {
  if (array_size == 0) {
    return false;
  }

  int mid{(array_size - 1) / 2 + 1};
  bool is_find{false};

  if (mid != array_size) {
    if (needle < *(haystack + mid - 1)) {
      is_find = BinarySearch(needle, haystack, mid - 1);
    } else if (needle > *(haystack + mid - 1)) {
      is_find = BinarySearch(needle, haystack + mid, array_size - mid);
    }
  }

  if (needle == *(haystack + mid - 1)) {
    is_find = true;
  }

  return is_find;
}

int main() {
  ofstream fout("sequence.txt", ios::out | ios::trunc);

  if (!fout.is_open()) {
    cout << "File sequence.txt is not created" << endl;
    return 1;
  }

  int number_elements;

  cout << "Input number of elemnt for generation: ";
  cin >> number_elements;

  for (int i{0}; i < number_elements; i++) {
    fout << rand() % (MAX_RANDOM - MIN_RANDOM + 1) + MIN_RANDOM << " ";
  }

  fout.close();

  ifstream fin("sequence.txt");
  int *sequence = new int[number_elements];
  int *sequence_ptr{sequence};

  while (fin) {
    fin >> *sequence_ptr++;
  };

  cout << "Generated sequence:" << endl;
  PrintArray(sequence, number_elements);
  qsort(sequence, number_elements, sizeof(int), Compare);

  int needle;
  cout << "Input search number: ";
  cin >> needle;
  if (BinarySearch(needle, sequence, number_elements)) {
    cout << "Found :)" << endl;
  } else {
    cout << "Not found :(" << endl;
  }

  return 0;
}
