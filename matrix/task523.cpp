#include <climits>
#include <iostream>

using namespace std;

enum SortDirection {
  ASC,
  DESC,
};

struct Diff {
  int diff = INT_MAX;
  int el_min;
  int el_max;
  int index_min;
  int index_max;

  // Перегрузка операторов меньше(<) и больше(<).
  // В данном случае это позволяет переменную с типом Diff сравнивать с
  // переменной типа int
  bool operator<(int const &right) const { return this->diff < right; }
  bool operator>(int const &right) const { return this->diff > right; }
};

void GnomSort(int *arr, int len, SortDirection direct) {
  int i{1}, j{2};

  while (i < len) {
    if ((direct == SortDirection::DESC && *(arr + i - 1) > *(arr + i)) ||
        (direct == SortDirection::ASC && *(arr + i - 1) < *(arr + i))) {
      i = j;
      j++;
    } else {
      swap(*(arr + i - 1), *(arr + i));
      i--;
      if (i == 0) {
        i = j;
        j++;
      }
    }
  }
}

bool InArray(int needle, int *haystack, int len) {
  for (int i{0}; i < len; i++) {
    if (*(haystack + i) == needle) {
      return true;
    }
  }

  return false;
}

int main() {
  int n;
  cout << "Input n: ";
  cin >> n;

  int count_numbers{2 * n};
  int a[count_numbers], b[n], c[n], d[n];

  for (int i{0}; i < count_numbers; i++) {
    a[i] = rand() % (1000 - 1 + 1) + 1;
  }

  GnomSort(a, count_numbers, SortDirection::ASC);

  Diff diff;
  int used_index[count_numbers];
  for (int i{0}; i < count_numbers; i++) {
    used_index[i] = -1;
  }

  int k{0};

  for (int i{0}; i < n; i++) {
    for (int j{0}; j < count_numbers - 1; j++) {
      if (InArray(j, used_index, 2 * i)) {
        continue;
      }

      k = j + 1;
      while (InArray(k, used_index, 2 * i)) {
        k++;
      }
      if (k > count_numbers - 1) {
        break;
      }

      if (diff > a[k] - a[j]) {
        diff.diff = a[k] - a[j];
        diff.el_min = a[j];
        diff.el_max = a[k];
        diff.index_min = j;
        diff.index_max = k;
      }
    }
    b[i] = diff.el_max;
    c[i] = diff.el_min;
    d[i] = diff.diff;
    used_index[2 * i] = diff.index_min;
    used_index[2 * i + 1] = diff.index_max;
    diff.diff = INT_MAX;
  }

  for (int i{0}; i < count_numbers; i++) {
    cout << a[i] << " ";
  }
  cout << endl;

  for (int i{0}; i < n; i++) {
    cout << b[i] << "\t";
  }
  cout << endl;

  for (int i{0}; i < n; i++) {
    cout << c[i] << "\t";
  }
  cout << endl;

  for (int i{0}; i < n; i++) {
    cout << d[i] << "\t";
  }
  cout << endl;

  return 0;
}
