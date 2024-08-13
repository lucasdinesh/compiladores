
printNumber:.string	"%d\n"
printSTR: .string "%s\n"
	
	
	.globl	main

main:

## TAC_BEGIN
	pushq	%rbp

## TAC_PRINT
	movl	$777, a(%rip)
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	printNumber(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT

## TAC_END
	popq	%rbp
	ret

##DATA SECTION
	.bss
a: .long	4
	.section	.rodata
