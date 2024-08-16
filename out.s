## FIXED INIT
printNumber:.string	"%d\n"
printSTR: .string "%s\n"
printReal: .string "%f\n"
printChar: .string "%c\n"

## TAC_BEGINFUN
	.globl main
main:
	pushq %rbp

## TAC_GREAT
	movl	_c(%rip), %eax
	cmpl	_3(%rip), %eax
	setg    %al
	movzbl  %al, %eax
	movl	 %eax,_mYWeeirT_emp0(%rip)

	movl _mYWeeirT_emp0(%rip), %eax
	movl $1, %edx
	andl %eax, %edx
	jz .mYLabe_l0
## TAC_PRINT_STRING
	leaq _tempStringName_l17(%rip), %rax
	movq	%rax, %rsi
	leaq printSTR(%rip), %rax
	movq %rax, %rdi
	call printf@PLT

	jmp .mYLabe_l1
.mYLabe_l0:
## TAC_PRINT_STRING
	leaq _tempStringName_l18(%rip), %rax
	movq	%rax, %rsi
	leaq printSTR(%rip), %rax
	movq %rax, %rdi
	call printf@PLT

.mYLabe_l1:
## TAC_ENDFUN
	popq %rbp
	ret

##DATA SECTION
	.data
_tempStringName_l17:	.string	"c>4"
_a:	.byte	99
_b:	.long	1
_c:	.long	4
_tempStringName_l18:	.string	"C não é maior que 4"
_vector:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.long	10
_1:
	.long   1
_2:
	.long   2
_3:
	.long   3
_4:
	.long   4
_5:
	.long   5
_6:
	.long   6
_7:
	.long   7
_8:
	.long   8
_9:
	.long   9
_10:
	.long   10
_'c':
	.long   99
_mYWeeirT_emp0:
	.long  0
