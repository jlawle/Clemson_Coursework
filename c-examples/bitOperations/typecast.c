/* This Program Demonstrates the Use of Implicit Typecasting
*/
#include <stdio.h>

void main(void)
{ int i;

  signed char sc;
  unsigned char uc;
  short signed int ssi;
  short unsigned int usi;
  signed int si;
  unsigned int ui;
  float f;
  double d;
  long double ld;

  sc = si = 100000;  printf("sc = 1000000 = %d = %0X\n", sc, sc);
  sc = si = -100000;  printf("sc = -1000000 = %d = %0X\n", sc, sc);

  sc = si = 0x12345678;  printf("sc = 0x12345678 = %d = %0X\n", sc, sc);
  sc = si = -0x12345678;  printf("sc = -0x12345678 = %d = %0X\n", sc, sc);

  sc = f = 1.9999999;  printf("sc = 1.9999999 = %d = %0X\n", sc, sc);
  sc = f = -1.9999999;  printf("sc = -1.9999999 = %d = %0X\n", sc, sc);

  sc = f = 256.999999;  printf("sc = 256.9999999 = %d = %0X\n", sc, sc);
  sc = f = -256.9999999;  printf("sc = -256.9999999 = %d = %0X\n", sc, sc);

  printf("\n");
  
  uc = ui = 100000;  printf("uc = 1000000 = %d = %0X\n", uc, uc);
  uc = ui = -100000;  printf("uc = -1000000 = %d = %0X\n", uc, uc);

  uc = ui = 0x12345678;  printf("uc = 0x12345678 = %d = %0X\n", uc, uc);
  uc = ui = -0x12345678;  printf("uc = -0x12345678 = %d = %0X\n", uc, uc);

  uc = f = 1.9999999;  printf("uc = 1.9999999 = %d = %0X\n", uc, uc);
  uc = f = -1.9999999;  printf("uc = -1.9999999 = %d = %0X\n", uc, uc);

  uc = f = 256.999999;  printf("uc = 256.9999999 = %d = %0X\n", uc, uc);
  uc = f = -256.9999999;  printf("uc = -256.9999999 = %d = %0X\n", uc, uc);
  
  printf("\n");
  
  ssi = i = 100000;  printf("ssi = 1000000 = %d = %0X\n", ssi, ssi);
  ssi = i = -100000;  printf("ssi = -1000000 = %d = %0X\n", ssi, ssi);

  ssi = i = 0x12345678;  printf("ssi = 0x12345678 = %d = %0X\n", ssi, ssi);
  ssi = i = -0x12345678;  printf("ssi = -0x12345678 = %d = %0X\n", ssi, ssi);

  ssi = f = 1.9999999;  printf("ssi = 1.9999999 = %d = %0X\n", ssi, ssi);
  ssi = f = -1.9999999;  printf("ssi = -1.9999999 = %d = %0X\n", ssi, ssi);

  ssi = f = 1.0E5;  printf("ssi = 1.0E5 = %d = %0X\n", ssi, ssi);
  ssi = f = -1.0E5;  printf("ssi = -1.0E5 = %d = %0X\n", ssi, ssi);

  printf("\n");
  
  usi = i = 100000;  printf("usi = 1000000 = %d = %0X\n", usi, usi);
  usi = i = -100000;  printf("usi = -1000000 = %d = %0X\n", usi, usi);

  usi = i = 0x12345678;  printf("usi = 0x12345678 = %d = %0X\n", usi, usi);
  usi = i = -0x12345678;  printf("usi = -0x12345678 = %d = %0X\n", usi, usi);

  usi = f = 1.9999999;  printf("usi = 1.9999999 = %d = %0X\n", usi, usi);
  usi = f = -1.9999999;  printf("usi = -1.9999999 = %d = %0X\n", usi, usi);

  usi = f = 1.0E5;  printf("usi = 1.0E5 = %d = %0X\n", usi, usi);
  usi = f = -1.0E5;  printf("usi = -1.0E5 = %d = %0X\n", usi, usi); 

  printf("\n");
  
  f = 1.23456789012345678E-20;  printf("f = 1.23456789012345678E-20 = %.20E\n", f);
  f = 1.23456789012345678E-20L;  printf("f = 1.23456789012345678E-20L = %.20LE\n", f);

  f = 1.23456789012345678E-50;  printf("f = 1.23456789012345678E-50 = %.20E\n", f);
  f = 1.23456789012345678E-50L;  printf("f = 1.23456789012345678E-50L = %.20LE\n", f);

  f = 1.23456789012345678E-500;  printf("f = 1.23456789012345678E-500 = %.20E\n", f);
  f = 1.23456789012345678E-500L;  printf("f = 1.23456789012345678E-500L = %.20LE\n", f);

  printf("\n");

  d = 1.23456789012345678E-50;  printf("d = 1.23456789012345678E-50 = %.20E\n", d);
  d = 1.23456789012345678E-50L;  printf("d = 1.23456789012345678E-50L = %.20LE\n", d);

  d = 1.23456789012345678E-500;  printf("d = 1.23456789012345678E-500 = %.20E\n", d);
  d = 1.23456789012345678E-500L;  printf("d = 1.23456789012345678E-500L = %.20LE\n", d);

  printf("\n");

  ld = 1.01234567890123456789012E-500;  printf("ld = 1.01234567890123456789012E-500 = %.22E\n", ld);
  ld = 1.01234567890123456789012E-500L;  printf("ld = 1.01234567890123456789012E-500L = %.22LE\n", ld);
  
  ld = 1.01234567890123456789012E-5000;  printf("ld = 1.01234567890123456789012E-5000 = %.22E\n", ld);
  ld = 1.01234567890123456789012E-5000L;  printf("ld = 1.01234567890123456789012E-5000L = %.22LE\n", ld);

  printf("\n");
}
