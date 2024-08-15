## FIXED INIT
printNumber:.string	"%d\n"
printSTR: .string "%s\n"
printReal: .string "%f\n"
printChar: .string "%c\n"

## TAC_BEGINFUN
	.globl main
main:
	pushq %rbp

## TAC_PRINT_STRING
	leaq _tempStringName_l17(%rip), %rax
	movq	%rax, %rsi
	leaq printSTR(%rip), %rax
	movq %rax, %rdi
	call printf@PLT

## TAC_PRINT_BOOL 1 is TRUE 0 is False
	movl	_b(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_PRINT_INT
	movl	_75(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_PRINT_CHAR
movzbl	_a(%rip), %eax
movsbl	%al, %eax
movl	%eax, %esi
leaq	printChar(%rip), %rax
movq	%rax, %rdi
	call	printf@PLT

## TAC_ADD
	movl	_c(%rip), %edx
	movl	_6(%rip), %eax
	addl	%edx, %eax
## TAC_COPY
	movl	%eax, _c(%rip)

## TAC_PRINT_INT
	movl	_c(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_VECATTR
	xorl	%eax, %eax
	movl	_999(%rip), %esi
	movl	 %esi, _vector+28(%rip)
	movl	%esi, (%rcx,%rdx,4)

## TAC_VEC_CALL
	xorl	%eax, %eax
	movq	_mYWeeirT_emp1(%rip), %rcx
	movl	_vector+8(%rip), %edx
	movl	%edx, (%rcx)

## TAC_ADD
	movl	_mYWeeirT_emp1(%rip), %edx
	movl	_5(%rip), %eax
	addl	%edx, %eax
## TAC_VECATTR
	xorl	%eax, %eax
	movl	_mYWeeirT_emp2(%rip), %esi
	movl	 %esi, _vector+28(%rip)
	movl	%esi, (%rcx,%rdx,4)

## TAC_VEC_CALL
	xorl	%eax, %eax
	movq	_mYWeeirT_emp3(%rip), %rcx
	movl	_vector+8(%rip), %edx
	movl	%edx, (%rcx)

## TAC_PRINT_INT
	movl	_mYWeeirT_emp3(%rip), %eax
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
_tempStringName_l17:	.string	"Digite algo"
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
_75:
	.long   75
_mYWeeirT_emp0:
	.long  0
_mYWeeirT_emp1:
	.long  0
_mYWeeirT_emp2:
	.long  0
_mYWeeirT_emp3:
	.long  0
