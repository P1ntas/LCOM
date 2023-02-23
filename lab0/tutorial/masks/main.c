#include <stdio.h>

/* int main(int argc, char *argv[])
{
  unsigned char c;

  // definir o valor inicial de c
  c = 95;
  printf("c: %d (%c)\n", c, c);

  // alterar o valor de c
  c = 100;
  printf("c: %d (%c)\n", c, c);

  return 0;
} */


// AND '&'

int main(int argc, char *argv[])
{
  unsigned char byte = 0x38; // 0011 1000
  unsigned char mask = 0xA9; // 1010 1001

  byte = byte & mask; //    0011 1000
                      //  & 1010 1001
                      // -------------
                      //    0010 1000
  
  printf("byte: 0x%02x\n", byte);
  // byte <- 0010 1000 (0x28)

  return 0;
}

// OR '|'

#include <stdio.h>

int main(int argc, char *argv[])
{
  unsigned char byte = 0x38; // 0011 1000
  unsigned char mask = 0xA9; // 1010 1001

  byte = byte | mask; //    0011 1000
                      //  | 1010 1001
                      // -------------
                      //    1011 1001
  
  printf("byte: 0x%02x\n", byte);
  // byte <- 1011 1001 (0xB9)

  return 0;
}

// XOR '^'

#include <stdio.h>

int main(int argc, char *argv[])
{
  unsigned char byte = 0x38; // 0011 1000
  unsigned char mask = 0xA9; // 1010 1001

  byte = byte ^ mask; //    0011 1000
                      //  ^ 1010 1001
                      // -------------
                      //    1001 0001
  
  printf("byte: 0x%02x\n", byte);
  // byte <- 1001 0001 (0x91)

  return 0;
}