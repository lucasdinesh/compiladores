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
	leaq tempStringName_l16(%rip), %rax
	movq	%rax, %rsi
	leaq printSTR(%rip), %rax
	movq %rax, %rdi
	call printf@PLT

## TAC_PRINT_BOOL 1 is TRUE 0 is False
	movl	b(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_ENDFUN
	popq %rbp
	ret

##DATA SECTION
	.data
a:	.byte	99
b:	.long	1
	.section	 .rodata
tempStringName_l16:	.string	"Digite algo"
vector:
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
