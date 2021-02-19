/* begin assembly stub */
.globl AtoI
.type AtoI,@function
AtoI:
/* prolog */
pushl %ebp
movl %esp, %ebp
pushl %ebx
pushl %esi
pushl %edi
/* put code here */

movl $1, sign

Spaces:
    movl ascii, %eax
    cmpb $32, (%eax)    #compares " " to *ascii
    je increment
    cmpb $9, (%eax)     #compares \t to *ascii
    je increment
    jmp plusMinus

increment:            ##this increments ascii to the next space if theres space
    addl $1, ascii
    jmp Spaces

plusMinus:
    cmpb $43, (%eax)    #compares + to *ascii by dereferencing register A
    je incrementPlus    #goes to increment immediately if if == true
    cmpb $45, (%eax)    #compares - to *ascii
    jne intPtrZero      #skips over the increment if neither condition satisfies the if-else
    movl $-1, sign      #makes sign negative if ifelse == true

incrementPlus:
    addl $1, ascii        #increments ascii if + or - is found

intPtrZero:
    movl intptr, %eax     #sets the intptr to 0
    movl $0, (%eax)


movl $0, i                #sets i = 0 for for loop
ForLoop:                         #for loop that puts us at the ones place
    movl $0, %eax                #clears out register to avoid overflow
    movl ascii, %ebx
    movl i, %edi
    movb (%ebx, %edi, 1), %al    #ascii[i] is placed in register A
    cmpb $48, %al                #if ascii[i] is ~<= 0 is true, will jump
    jnge exit
    cmpb $57, %al                #if ascii[i] ~>=9 is true, will jump
    jnle exit
    addl $1, i                   #increments i
    jmp ForLoop                  #restarts loop

exit:

addl $-1, i
movl $1, multiplier

SecondForLoop:                    #this is the second for loop
    movl $0, %ebx                 #clears register B
    movl i, %edi                  #move i into the index register
    cmpl $0, i
    jl TheEnd                     #skips out of loop if i is less than 0
    movl ascii, %ebx
    movb (%ebx, %edi, 1), %al     #moves ascii[i] into register A
    subb $48, %al                 #subtracts '0' from ascii
    mull multiplier               #multiplies ascii in register A by multiplier
    movl intptr, %ebx             #puts intptr into ebx
    addl %eax, (%ebx)             #adds eax to the intptr in %ebx
    movl  multiplier, %eax
    movl $10, %ebx
    mull %ebx                     #multiplies 10 by multiplier
    movl %eax, multiplier         #sets multiplier to factor of 10
    addl $-1, i                   #de increment i
    jmp SecondForLoop

TheEnd:                  #ending part that derefs intptr and corrects the sign
    movl intptr, %ebx
    movl (%ebx), %eax
    mull sign                #mutliplies the sign into intptr
    movl %eax, (%ebx)

return:
/* epilog */
popl %edi
popl %esi
popl %ebx
movl %ebp, %esp
popl %ebp
ret
/* end assembly stub */
