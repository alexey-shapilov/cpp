// Упражнение 5.3.
// Используя указатели, напишите функцию strcat, которую мы рассматривали в гл.
// 2 (функция strcat(s, t) копирует стринг t в конец стринга s).

#include <iostream>

void my_strcat(char *destptr, const char *srcptr) {
  while (*destptr++)
    ;

  destptr--;
  while ((*destptr++ = *srcptr++))
    ;
}

int main() {
  char str[100] = "Hello, ";

  my_strcat(str, "World\n");

  std::cout << str;

  return 0;
}
