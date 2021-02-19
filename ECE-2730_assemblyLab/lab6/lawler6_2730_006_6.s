/* John Lawler
ECE 2730
April 17th 2019 (oops) */


/* begin assembly stub */
/* put assembler directives here */
.globl Factorial
.type Factorial, @function

Factorial:
  /* prolog */
  pushl %ebp
  pushl %ebx
  movl %esp, %ebp
  subl $4, %esp       # allocates space for one local var

  /* put code here */
  movl 12(%ebp), %ecx       # increments to address 3 rows down, past return addr and stores n in A reg
  cmpl $0, %ecx
  je finish
  cmpl $1, %ecx
  je finish               # two compare statements for n == 0 and n == 1, finishes loop if true

  # else  statement
  movl %ecx, (%esp)     # moves variable onto stack to store for later
  subl $1, %ecx         # n - 1
  pushl %ecx            #caller responsbility pushing ecx into the function
  call Factorial
  movl (%esp), %ecx     # move the original n into eax to multiply
  mull %ecx           # multiplies the return value left in reg A from Calling factorial with n in ecx
  jmp return

  finish:
    movl $1, %eax     # places a 1 in the return register A

  return:
  /* epilog */
  movl %ebp, %esp      # reset top of stack pointer to deallocate memory above basepointer
  popl %ebx
  popl %ebp


  ret
/* end assembly stub */
