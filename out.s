## FIXED INIT
printNumber:.string	"%d\n"
printSTR: .string "%s\n"
printReal: .string "%f\n"
printChar: .string "%c\n"

## TAC_BEGINFUN
	.globl main
main:
	pushq %rbp

## TAC_VEC_CALL
	movl	_vector+28(%rip), %edx
	movl	%edx, _mYWeeirT_emp0(%rip)

## TAC_PRINT_INT
	movl	_mYWeeirT_emp0(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_VECATTR
	xorl	%eax, %eax
	movl	_999(%rip), %esi
	movl	 %esi, _vector+28(%rip)

## TAC_VEC_CALL
	movl	_vector+28(%rip), %edx
	movl	%edx, _mYWeeirT_emp1(%rip)

## TAC_PRINT_INT
	movl	_mYWeeirT_emp1(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_ENDFUN
	popq %rbp
	ret

##DATA SECTION
	.data
_a:	.byte	99
_b:	.long	1
_c:	.long	4
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
_999:
	.long   999
_10:
	.long   10
_'c':
	.long   99
_mYWeeirT_emp0:
	.long  0
_mYWeeirT_emp1:
	.long  0
