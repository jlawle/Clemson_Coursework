/* This Program Demonstrates the precision  
   of the ieee 754 specification
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float f;
double d;
long double ld;

void main(void)
{ 
  f = d = ld = 1; 
  
  printf("f = %.20e, d = %.20e, ld = %.20Le\n\n", f, d, ld);
  getchar();
   
  do
  { f /= 10; 
    printf("f = %.20e\n", f);
  } while (f > 0);
  getchar();
    
  do 
  { d /= 100;
    printf("d = %.20e\n", d);
  } while (d > 0);
  getchar();
  
  do 
  { ld /= 10000;
    printf("ld = %.20Le\n", ld);
  } while (ld > 0); 
  getchar();
    
  /******************************/
  
  f = d = ld = 1; 

  do
  { f *= 10;
    printf("f = %.20E\n", f);
  } while (f < INFINITY);
  printf("\n");
  
  getchar();
    
  do
  { d *= 100;
    printf("d = %.20E\n", d);
  } while (d < INFINITY);
  printf("\n");
  
  getchar();
    
  do
  { ld *= 10000;
    printf("ld = %.20LE\n", ld);
  } while (ld < INFINITY);
  printf("\n");
}
