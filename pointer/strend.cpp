// Упражнение 5.4.
// Напишите функцию strend(s, t), которая выдает 1, если стринг t расположен в
// конце стринга s, и нуль в противном случае.

#include <iostream>

int strend(char *haystack, const char *needle) {
  while (*haystack != *needle) {
    haystack++;
  }

  while ((*haystack == *needle) && (*haystack != '\0' || *needle != '\0')) {
    haystack++;
    needle++;
  }

  return (*needle == '\0' && *haystack == '\0') ? 1 : 0;
}

int main() {
  char haystack[] = "Hello, World";

  std::cout << strend(haystack, "World") << std::endl;
  std::cout << strend(haystack, "llo") << std::endl;
  std::cout << strend(haystack, "ololo") << std::endl;

  return 0;
}
