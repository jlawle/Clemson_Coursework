/* begin assembly stub */
.globl classify
.type classify,@function
classify:
/* prolog */
pushl %ebp
pushl %ebx
movl %esp, %ebp
/* put code here */

movl i, %eax		#block  of checks for 0 = i,j,k
cmpl $0, %eax
jne NotTriangle
movl j, %eax
cmpl $1, %eax
jne NotTriangle
movl k, %eax
cmpl $1, %eax
jne NotTriangle
movl $0, tri_type
jmp return

NotTriangle: 	#jumps over the case of any equalling 0

movl $0, match		#block if(i ==j)
movl i, %eax
cmpl j, %eax				#compare j to i
jne NoMatch					#skips if i != j
addl $1, match

NoMatch:
	cmpl k, %eax				#compares k to i in register a
	jne NotBigMatch
	addl $2, match

NotBigMatch:
	movl j, %eax
	cmpl k, %eax
	jne NextBigThing
	addl $3, match

NextBigThing:
	movl match, %eax
	cmpl $0, %eax
	je Wow_this_aint_nuthin

cmpl $1, %eax								#checks match = 1
jne elseBigMoney						#skips to else statement of (match == 1)
movl i, %eax								#start of (i + j) <= k statement
addl j, %eax
cmpl %eax, k
jle notElseLessMoney				#jump to isoceles if ((i +j) <= k) is false
movl $0, tri_type						#state its a triangle if above was true
jmp return

elseBigMoney:
	movl match, %eax
	cmpl $2, %eax							#checks if match != 2
	jne elseBiggerMoneyBags 	#goes to check if (match == 6) if match != 2
	movl $2, tri_type					#labels it isoceles triangle
	jmp return

elseBiggerMoneyBags:
	movl match, %eax
	cmpl $6, %eax
	jne elseElseBiggerMoneyBags		#if match != 6, jumps over return code below
	movl $1, tri_type				  		#labels it as equalateral triangle
	jmp return

elseElseBiggerMoneyBags:
	movl j, %eax
	addl k, %eax
	cmpl i, %eax								#checks the j+k <= i
	jg ElseLesserMoneys					#goes to check the next statement if false
	movl $0, tri_type
	jmp return

ElseLesserMoneys:
	movl i, %eax
	addl k, %eax
	cmpl j, %eax								#checking the i + k <= j statement
	jg notElseLessMoney					#jumps to end of match == 1 statement
	movl $0, tri_type
	jmp return

notElseLessMoney:							#end of (match == 1)
	movl $2, tri_type
	jmp return

Wow_this_aint_nuthin: 	#this goes to after end of " if (match) statement
	movl i, %eax
	addl j, %eax
	cmpl k, %eax					# i + j <= k comparison
	jle stillNothing
	movl j, %eax
	addl k, %eax
	cmpl i, %eax					# j + k <= i comparison
	jle stillNothing
	movl i, %eax
	addl k, %eax
	cmpl j, %eax					# j + k <= i comparison
	jle stillNothing
	movl $3, tri_type			#if none are true for the statement, will give a 3 for tri_type
	jmp return

stillNothing:
		movl $0, tri_type
		jmp return



return:

/* epilog */
movl %ebp, %esp
popl %ebx
popl %ebp
ret
/* declare variables here */
.comm match, 4
/* end assembly stub */
