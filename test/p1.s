
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

#TAC_ADD
	
	movl	a(%rip), %edx
	movl	b(%rip), %eax
	addl	%edx, %eax

#TAC_SUB
	movl	a(%rip), %eax
	movl	b(%rip), %edx
	subl	%edx, %eax

#TAC_DIV
	movl	a(%rip), %eax
	movl	b(%rip), %ecx
	cltd
	idivl	%ecx

#TAC_MUL
	movl	a(%rip), %edx
	movl	b(%rip), %eax
	imull	%edx, %eax
## TAC_END
	popq	%rbp
	ret

##DATA SECTION
	.bss
a: .long	4
	.section	.rodata
