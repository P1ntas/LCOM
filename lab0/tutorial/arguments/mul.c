#include <stdio.h>
#include <stdlib.h>

int isNumber(char number[])
{
    int i = 0;
    while (number[i] != '\0')
    {
        if (number[i] < '0' || number[i] > '9')
        return 0;
        i++;
    }
    return 1;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("erro: sintaxe invalida!\n");
    return 1;
  }
  
  int num1, num2;

  if (!isNumber(argv[1]) || !isNumber(argv[2]))
  {
    printf("erro: argumentos invalidos!\n");
    return 1;
  }

  num1 = atoi(argv[1]);
  num2 = atoi(argv[2]);
  printf("resultado: %d\n", num1 * num2);
  
  return  0;
}