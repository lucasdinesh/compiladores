	.file	"t1.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	8
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	7
	.globl	d
	.align 4
	.type	d, @object
	.size	d, 4
d:
	.long	1089889894
	.globl	e
	.align 4
	.type	e, @object
	.size	e, 4
e:
	.long	1090051375
	.globl	c
	.type	c, @object
	.size	c, 7
c:
	.string	"string"
	.globl	f
	.type	f, @object
	.size	f, 1
f:
	.byte	99
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:

	pushq	%rbp

	movl	b(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	
	popq	%rbp
	ret

.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
