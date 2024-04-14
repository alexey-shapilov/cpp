// Упражнение 5.2.
// Напишите функцию getfloat — аналог getint для чисел с плавающей точкой. Какой
// тип будет иметь результирующее значение, выдаваемое функцией getfloat?

#include <cstdio>
#include <ctype.h>
#include <iostream>

#define SIZE 5

int getch(void);
void ungetch(int);

int main(int argc, char *argv[]) {
  float array[SIZE];
  float getfloat(float *);

  for (int i{0}; i < SIZE && getfloat(&array[i]) != EOF; i++)
    ;

  for (int i{0}; i < SIZE; i++) {
    std::cout << array[i] << "\t";
  }
  std::cout << std::endl;

  return 0;
}

float getfloat(float *pn) {
  char c;
  int sign;

  while (isspace(c = getch()))
    ; /* пропуск пробельных литер */
  if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
    ungetch(c); /* не число */
    return 0;
  }
  sign = (c == '-') ? -1 : 1;
  if (c == '+' || c == '-') {
    c = getch();
    if (!isdigit(c)) {
      ungetch(c);
      ungetch((sign == -1) ? '-' : '+');
      return 0;
    }
  }
  for (*pn = 0; isdigit(c); c = getch())
    *pn = 10 * *pn + (c - '0');

  if (c == '.') {
    c = getch();
    int devider = 1;
    float real{0};
    for (; isdigit(c); c = getch()) {
      real = 10 * real + (c - '0');
      devider *= 10;
    }
    *pn = *pn + real / devider;
  }
  *pn *= sign;

  if (c != EOF)
    ungetch(c);

  return c;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* буфер для ungetc */
int bufp = 0; /* след. свободная позиция в буфере */

int getch(void) /* дай (возможно возвращенную) литеру */
{
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* верни литеру на ввод */
{
  if (bufp >= BUFSIZE)
    printf("ungetch: слишком много литер\n");
  else
    buf[bufp++] = c;
}
