/* begin assembly stub */
.globl dodiff
.type dodiff, @function
dodiff:
/* prolog */
pushl %ebp
pushl %ebx
movl %esp, %ebp
/* put code here */

movl digit1, %eax 
mull %eax 	 	#moves and multiplies digit1 by itself
movl %eax, %ebx		#moves the number into b
movl digit2, %eax 	
mull %eax		#moves and multiples digit2 by itself
movl %eax, %ecx	
addl %ebx, %ecx		#sums the digit1 and digit2 products and places it in c
movl digit3, %eax
mull %eax
subl %eax, %ecx		#subtracts a storing the digit3 product from c, the sum
movl %ecx, diff		#places the end result in diff






/* epilog */
movl %ebp, %esp
popl %ebx
popl %ebp
ret
.globl dosumprod
.type dosumprod, @function
dosumprod:
/* prolog */
pushl %ebp
pushl %ebx
movl %esp, %ebp
/* put code here */

movl digit1, %eax	#places digits in A,B,C
movl digit2, %ebx
movl digit3, %ecx
addl %eax, %ebx		#adds together A and B and palces in B
addl %ebx, %ecx		#adds together B and C and palces in C
movl %ecx, sum		#moves C into the variable sum

movl digit1, %eax
movl digit2, %ebx
movl digit3, %ecx
mull %ebx
mull %ecx
movl %eax, product





/* epilog */
movl %ebp, %esp
popl %ebx
popl %ebp
ret
.globl doremainder
.type doremainder, @function
doremainder:
/* prolog */
pushl %ebp
pushl %ebx
movl %esp, %ebp
/* put code here */

movl product, %eax
movl $0, %edx
divl sum		#divides product in a by sum
movl %edx, remainder	





/* epilog */
movl %ebp, %esp
popl %ebx
popl %ebp
ret
/* declare variables here */
.comm digit1, 4
.comm digit2, 4
.comm digit3, 4
.comm diff, 4
.comm sum, 4
.comm product, 4
.comm remainder, 4

/* end assembly stub */
/* Do not forget the required blank line here! */

