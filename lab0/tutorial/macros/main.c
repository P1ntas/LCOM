#include <stdio.h>

/*-------------------------NOT USING MACRO-------------------------*/

int is_number(char c)
{
  if (c >= '0' && c <= '9')
    return 1;
  
  return 0;
}

/*---------------------USING MACRO BELOW---------------------------*/

#define TRUE    1
#define FALSE   0

int is_number(char c)
{
  if (c >= '0' && c <= '9')
    return TRUE;

  return FALSE;
}

/*----------------------------EXAMPLE 2----------------------------*/

#define PI 3.1415926

int main(int argc, char *argv[])
{
  float raio = 2.0f;
  
  printf("A area do circulo é: %f\n", PI * raio * raio);
  printf("O perimetro do circulo é: %f\n", 2 * PI * raio);

  return 0;
}