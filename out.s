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
	leaq tempStringName_l9(%rip), %rax
	movq	%rax, %rsi
	leaq printSTR(%rip), %rax
	movq %rax, %rdi
	call printf@PLT

## TAC_PRINT_INT
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
## TAC_ENDFUN
	popq %rbp
	ret

##DATA SECTION
a:	.long	6
c:	.long	99
d:	.long	49
f:	.float	2.5
tempStringName_l9:	.string	"Digite"
	.section	.rodata
