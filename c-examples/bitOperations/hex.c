/* This Program Demonstrates the use of Hexadecimal numbers
*/  
#include <stdio.h>

#define UCHAR unsigned char

char *HexToBin[16] =
{ "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
  "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

void print_bits(char *text, void *mem, unsigned char len)
{ unsigned char *addr;
  unsigned char i;

  // *** Print Binary Value of Memory *** //
  printf(text);
  for (addr=(UCHAR *)mem + len - 1; addr>=(UCHAR *)mem; addr--)
  {  printf("%4s", &HexToBin[*addr>>4][0]);
     printf("%4s ", &HexToBin[*addr % 16][0]);
  }
  printf("\n");
}

//---------------------------------------------------------------------------

#define m 0x1234ABCD1234ABCD
#define n (9223372036854775808L + 2147483648 + 32768 + 128)

void main(void)
{ unsigned char c; 
  unsigned short int si;
  unsigned int i;
  unsigned long l;
   
  c =  m; print_bits("c  = ",&c, sizeof(c));
  si = m; print_bits("si = ",&si, sizeof(si));
  i =  m; print_bits("i  = ",&i, sizeof(i));
  l =  m; print_bits("l  = ",&l, sizeof(l));
  printf("\n");

  c =  n; printf("c  = %d = 0x%X", c, c);
  print_bits(" = ",&c, sizeof(c));
  si = n; printf("si = %d = 0x%X", si, si);
  print_bits(" = ",&si, sizeof(si));
  i =  n; printf("i  = %d = 0x%X", i, i);
  print_bits(" = ",&i, sizeof(i));
  l =  n; printf("l  = %ld = 0x%lX", l, l);
  print_bits(" = ",&l, sizeof(l));
}
