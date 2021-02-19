/* This Program Demonstrates 2's Complement Numbers
   and Arithmetic
*/
#include <stdio.h>

#define UCHAR unsigned char

char *HexToBin[16] =
{ "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
  "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

void print_bits(void *mem, unsigned char len)
{ unsigned char *addr;
  unsigned char i;

  // *** Print Binary Value of Memory *** //
  for (addr=(UCHAR *)mem + len - 1; addr>=(UCHAR *)mem; addr--)
  {  printf("%4s", &HexToBin[*addr>>4][0]);
     printf("%4s ", &HexToBin[*addr % 16][0]);
  }
  printf("\n");
}

//---------------------------------------------------------------------------

void main(void)
{  char a, b, c;
   
   a = 23;     printf("a = %4d = ", a); print_bits(&a, sizeof(a));
   b = 17;     printf("b = %4d = ", b); print_bits(&b, sizeof(b));
   c = a + b;  printf("c = %4d = ", c); print_bits(&c, sizeof(c));
   
   printf("\n");

   a = -23;    printf("a = %4d = ", a); print_bits(&a, sizeof(a));
   b = -17;    printf("b = %4d = ", b); print_bits(&b, sizeof(b));
   c = a + b;  printf("c = %4d = ", c); print_bits(&c, sizeof(c));

   printf("\n");
   
   a = 64;     printf("a = %4d = ", a); print_bits(&a, sizeof(a));
   b = 64;     printf("b = %4d = ", b); print_bits(&b, sizeof(b));
   c = a + b;  printf("c = %4d = ", c); print_bits(&c, sizeof(c));
}
