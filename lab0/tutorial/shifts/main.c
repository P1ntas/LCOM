#include <stdio.h>
#include <assert.h>

int is_bit_um(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < num_bits_int );
  int mascara_num_bit = (1 << num_bit);
  return ( valor & mascara_num_bit );
}

int main(int argc, char *argv[])
{
  int hex = 0xF0F1F2F3;
  int bit_i, i;
  int total_bits = sizeof(hex) * 8;
 
  printf("Conversão de 0x%X:\n",hex);
  for(i=total_bits-1;i>=0;i--) {
    bit_i = is_bit_um(hex, i) ? 1 : 0;
    printf("%d",bit_i);
  }
  printf("\n");
 
  return 0;
}