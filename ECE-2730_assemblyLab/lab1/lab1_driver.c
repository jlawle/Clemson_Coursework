/* begin C driver */
/* begin assembly code */
/* NAME     John Lawler
   Course   ECE 273	
   SECTION  006
   DATE     Jan 23 2019
   FILE     lab1_driver.c
   PURPOSE  this assembly is used to add 
	    together ascii digits from a string 
	    sent to it
*/

#include <stdio.h>
int main(int arg, char **argv)
{
char buffer[256];
do {
int i = 0;
printf ("Enter a string terminated with a newline\n");
do {
buffer[i] = getchar();
} while (buffer[i++] != '\n');
buffer[i-1] = 0;
/* asum() is the function implemented in assembly */

i = asum(buffer);
if (i) {
printf ("ascii sum is %d\n", i);
continue;
}
} while(1);
return 0;
}
/* end C driver */
