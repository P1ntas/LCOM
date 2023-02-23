#include <stdio.h>

/* int main(int argc, char *argv[])
{
  printf("argc: %d\n", argc);
  printf("argv: %s\n", argv[0]);
  return 0;
} */

int main(int argc, char *argv[])
{
  int i;
  for (i = 0; i < argc; i++)
    printf("%d Parametro: %s\n", i, argv[i]);
  return  0;
}