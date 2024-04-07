#include <iostream>
#include <ostream>

void my_strcat(char *destptr, const char *srcptr) {
  while (*destptr++)
    ;

  destptr--;
  while ((*destptr++ = *srcptr++))
    ;
}

int main() {
  char str1[8] = "Hello, ";

  my_strcat(str1, "World\n");

  std::cout << str1;

  return 0;
}
