/* This Program Demonstrates the size of the different data types
   Using the sizeof Operator
 */

#include <stdio.h>

void print_range(char *text, int bytes)
{
  printf("%s", text);
  printf("%d\t%d\t",bytes,bytes*8);
  printf("\n");
}

/* ---------------------------------------------------------------------- */

void main(void)
{ struct
  { char c[56];
    int i[7];
  } TestStruct;

  /* int/char   are 'type specifiers'
     short/long are 'type qualifiers' */
  
  int i;
  printf("     Type\tBytes\tBits\n");
  print_range("  signed char \t", sizeof(signed char));
  print_range("         char \t", sizeof(char));
  print_range("       char * \t", sizeof(char *));
  print_range("unsigned char \t", sizeof(unsigned char));
  printf("\n");
  print_range("    short int \t", sizeof(short int));
  print_range("          int \t", sizeof(int));
  print_range("        int * \t", sizeof(int *));
  print_range("     long int \t", sizeof(long int));
  print_range("   long int * \t", sizeof(long int *));
  print_range("   signed int \t", sizeof(signed int));
  print_range(" unsigned int \t", sizeof(unsigned int));
  printf("\n");
  print_range("        float \t", sizeof(float));
  print_range("      float * \t", sizeof(float *));
  print_range("       double \t", sizeof(double));
  print_range("     double * \t", sizeof(double *));
  print_range("  long double \t", sizeof(long double));
  printf("\n");
  print_range("   TestStruct \t", sizeof(TestStruct));
  printf("\n\n\n\n");
}
