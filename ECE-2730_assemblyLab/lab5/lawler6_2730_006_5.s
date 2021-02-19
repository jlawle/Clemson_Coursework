.globl Fib
.type Fib,@function
Fib:
/* prolog */
pushl %ebp
pushl %ebx
movl %esp, %ebp
subl $8, %esp
/* put code here */

movl global_var, %eax     #sets global = local
movl %eax, local_var
movl %eax, -4(%ebp)       #places local onto the stack
cmpl $0, local_var         #if local = 0, jmp to return
je return
cmpl $1, local_var         #if local = 1, jmp to return
je return

movl -4(%ebp), %eax       #moves local var into regA to subtract 1
subl $1, %eax             #subtract 1 from local
movl %eax, global_var     #sets global = local - 1
call Fib
movl global_var, %eax     #sets global_var = temp_var
movl %eax, temp_var       #global = temp_var
movl %eax, -8(%ebp)       #places the temp var in the stack 4 bytes after local
movl -4(%ebp), %eax       #moves local var from stack into RegA
subl $2, %eax
movl %eax, global_var         #sets the global variable = local - 2
call Fib                      #second Fib call function
movl -8(%ebp), %eax           #move temp_var from stack into Areg to add global
addl global_var, %eax         #adds global to regA
movl %eax, temp_var           #places the resulting addition back into temp_var on stack
movl %eax, -8(%ebp)           #places tempvar into stack
movl %eax, global_var         #sets global_var = temp_var



return:
/* epilog */
movl %ebp, %esp
popl %ebx
popl %ebp
ret

.comm local_var, 4
.comm temp_var, 4
