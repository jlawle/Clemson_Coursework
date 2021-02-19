/* This Program Demonstrates the Practical Use of 
   Bitwise Operators
*/ 
#include <stdio.h>

/* This program writes the upper nibble and then the lower nibble of   */
/* a variable called a to bit 3, 4, 5, and 6 of variable PortA       */
/* using bitwise operations without changing the other bits of PortA */

#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

#define UCHAR unsigned char

char *HexToBin[16] =
{ "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
  "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
};

void print_bits(char *text, void *mem, UCHAR len)
{ UCHAR *addr;
  UCHAR i;

  /*** Print Binary Value of Memory ***/
  printf("%s", text);
  for (addr=(UCHAR *)mem + len - 1; addr>=(UCHAR *)mem; addr--)
  {  printf("%4s", &HexToBin[*addr>>4][0]);
     printf("%4s ", &HexToBin[*addr % 16][0]);
  }
  printf("\n");
}

/**************************************************************/

void main(void)
{ int i;
  unsigned char a;
  unsigned char PortA = 0xC1; /* Initialize PortA to current value
                                 and show other bits don't change
                                 when writing character nibbles  */
                               
  for (i=0; i<5; i++)
  { printf("Input a character to write to the display.\n");
    a = getchar();  
    getchar();  /*** Strip out Linefeed Character ***/
    printf("a = 0x%X\n", a);
    print_bits("a = ", &a, 1);
    
    /*** Port A before the writing of Data ***/
    printf("\nPortA before writing to LCD...\n");
    printf("PortA[0] = 0x%X\n", PortA);
    print_bits("PortA[0] = ", &PortA, 1);
    
    /* Write the upper nibble of a to pins (bits) 2-5 of PortA
       Clear bit positions to be written */
    PortA = PortA & ~(BIT_6 | BIT_5 | BIT_4 | BIT_3); 
    /* Then Or in the upper nibble of a to those positions.  */
    PortA = PortA |  ((a & 0xF0) >> 1) ;
    
    printf("\nPortA after writing Upper Nibble to LCD...\n");
    printf("PortA[1] = 0x%X\n", PortA);
    print_bits("PortA[1] = ", &PortA, 1);
    
    /* Write the lower nibble of a to pins (bits) 2-5 of PortA */
    /* Clear bit positions to be written */
    PortA = PortA & ~(BIT_6 | BIT_5 | BIT_4 | BIT_3); 
    /* Then Or in the lower nibble of a to those positions. */
    PortA = PortA | ((a & 0x0F) << 3) ;
    
    printf("\nPortA after writing Lower Nibble to LCD...\n");
    printf("PortA[2] = 0x%X\n", PortA);
    print_bits("PortA[2] = ", &PortA, 1);
    
    puts("\n");
  }
}
