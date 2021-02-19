/* This Program Demonstrates the Use of using Bitwise Operator Functions
   to Print Out Binary Numbers
*/
#include <stdio.h>

#define UCHAR unsigned char

void print_bits(char *text, void *mem, UCHAR len)
{ UCHAR *addr; int bit;

  // *** Print Binary Value of Memory *** //
  printf(text);
  for (addr=(UCHAR *)mem + len - 1; addr>=(UCHAR *)mem; addr--)
  {  for (bit = 7; bit>=0; bit--) printf((*addr >> bit) & 0x01 ? "1" : "0");
  }
  printf("\n");
}
//---------------------------------------------------------------------------

void main(void)
{  UCHAR i;

   printf("Enter a character...  "); 
   i = getchar();  fflush(stdin);
   
   printf("The binary value for %c is ");
   print_bits(" = ", &i, sizeof(i));
}
