// Упражнение 5.1.
// Функция getint написана так, что знаки - или +, за которыми не следует цифра,
// она понимает как «правильное» представление нуля. Скорректируйте программу
// таким образом, чтобы она в подобных случаях возвращала прочитанный знак назад
// во ввод.

// #include <cstdio>
// #include <ctype.h>
//
// int getch(void);
// void ungetch(int);
//
// /* getint: читает следующее целое из ввода в *pn */
// int getint(int *pn) {
//   int c, sign;
//
//   while (isspace(c = getch()))
//     ; /* пропуск пробельных литер */
//   if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
//     ungetch(c); /* не число */
//     return 0;
//   }
//   sign = (c == '-') ? -1 : 1;
//   if (c == '+' || c == '-')
//     c = getch();
//   for (*pn = 0; isdigit(c); c = getch())
//     *pn = 10 * *pn + (c - '0');
//   *pn *= sign;
//   if (c != EOF)
//     ungetch(c);
//
//   return c;
// }

#include <cstdio>
#include <ctype.h>
#include <iostream>

#define SIZE 5

int getch(void);
void ungetch(int);

int main(int argc, char *argv[]) {
  int n, array[SIZE], getint(int *);

  for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++)
    ;

  for (int i{0}; i < SIZE; i++) {
    std::cout << array[i] << "\t";
  }
  std::cout << std::endl;

  return 0;
}

/* getint: читает следующее целое из ввода в *pn */
int getint(int *pn) {
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
