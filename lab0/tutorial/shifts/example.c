#include <stdio.h>

#define BIT(n)   (1 << (n))

int main(int argc, char *argv[])
{
  char val;
  int n;

  printf("Insira o valor de val: ");    // 'a' = 0x61 ou 0b111101
  scanf("%c", &val);

  printf("Insira o valor de n: ");      // caso fosse 2, val = 0x63 ou 0b111111
  scanf("%d", &n);

  if (n < 0 || n > 7) {
    printf("error: valor de 'n' invalido [0..7]\n");
    return 1;
  }

  val = val | BIT(n);                   // aqui é feita a operação de OR
  printf("O novo valor de 'val': 0x%8x\n", val);
  
  return 0;
}