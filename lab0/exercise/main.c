#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
    return binstr;
}

void print_usage(char *name) {
    printf("Usage: %s <action> <byte> <bit no>\n", name);
    printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
        "\t\t <byte> 8-bit integer in hexadecimal\n"
        "\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[])
{
    char a;    // action: 'h', 'l', 'r', 's', 't'
    unsigned long n; // value to convert must be smaller than 256
    int bit; // bit to operate on: must be between 0 an 7
    
    { // a) Validate command line arguments 
        if (argc != 4) {
            print_usage(argv[0]);
            return 1;
        }
        
        // In C a string is essentially a null terminated character array
        a = argv[1][0];
        if( a != 'h' && a != 'l' && a != 'r' && a != 's' && a != 't' ) {
            print_usage(argv[0]);
            printf("\t<action> must be one of 'h'|'l'|'r'|'s'|'t'\n");
            return 1;
        }
    
        // We are expecting the argument to be an hexadecimal string
        n = strtoul(argv[2],NULL,16);
        if( n < 0 || n > (1 << 8) - 1) {
            print_usage(argv[0]);
            printf("\t<byte> must be non-negative and smaller than 256\n");
            return 1;
        }
        
        bit = atoi(argv[3]);
        if( bit < 0 || bit > 7 ) {
            print_usage(argv[0]);
            printf("\t<bit no> must be between 0 and 7\n");
            return 1;
        }    
    } // End of command line arguments validation
    
    char binary[9]; // array for binary representation of n,
    //  remember that in C strings are terminated with a 0
    unsigned long aux = n;

    // b) Print to stdout the binary representation of n
    printf("Binary representation of %lu is %s\n", n, byte2bin(n, binary));

    // c) + d) Do what the user asked and print the result

    // printing the name of the program
    printf("Program's name = %s\n", argv[0]);

    // switching on the action
    switch (a)
    {
    case 'h':           // h - testar se o valor de um determinado bit está a 1 (high)
        if (binary[7-bit] == '1')
        {
            printf("The bit %d is high (1)\n", bit);
        } else {
            printf("The bit %d is not high (0)\n", bit);
        }
        break;
    
    case 'l':           // l - testar se o valor de um determinado bit está a 0 (low)
        if (!(binary[7-bit] == '1'))
        {
            printf("The bit %d is low (0)\n", bit);
        } else {
            printf("The bit %d is not low (1)\n", bit);
        }
        break;

    case 'r':           // r - colocar a 0 o valor de um determinado bit (reset)
        if(binary[7-bit] == '1')
        {
            n &= ~(BIT(bit));
        }
        break;

    case 's':           // s - colocar a 1 o valor de um determinado bit (set)
        if(!(binary[7-bit] == '1'))
        {
            n |= BIT(bit);
        }
        break;

    case 't':           // t - inverter o valor de um determinado bit (toggle)
        n ^= BIT(bit);
        break;

    default:
        break;
    }
    
    // printing the final result after the action
    printf("Binary representation of %lu is %s\n", n, byte2bin(n, binary));
    
    return 0;
}