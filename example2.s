	.file	"example2.c"
	.text
	.p2align 4
	.globl	test
	.type	test, @function
test:
.LFB0:
	.cfi_startproc
	endbr64
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L2:
	vmovdqa	(%rdi,%rax), %ymm1
	vpmaxub	(%rsi,%rax), %ymm1, %ymm0
	vmovdqa	%ymm0, (%rdi,%rax)
	addq	$32, %rax
	cmpq	$65536, %rax
	jne	.L2
	vzeroupper
	ret
	.cfi_endproc
.LFE0:
	.size	test, .-test
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
