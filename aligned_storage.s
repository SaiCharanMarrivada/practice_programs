	.file	"aligned_storage.c"
	.text
	.p2align 4
	.globl	_Z3add7Vector4S_
	.type	_Z3add7Vector4S_, @function
_Z3add7Vector4S_:
.LFB546:
	.cfi_startproc
	endbr64
	movq	%xmm1, -32(%rsp)
	movq	%xmm0, -40(%rsp)
	movaps	-40(%rsp), %xmm4
	movq	%xmm2, -24(%rsp)
	movq	%xmm3, -16(%rsp)
	addps	-24(%rsp), %xmm4
	movaps	%xmm4, -40(%rsp)
	movq	-32(%rsp), %rax
	movq	-40(%rsp), %xmm0
	movq	%rax, %xmm1
	ret
	.cfi_endproc
.LFE546:
	.size	_Z3add7Vector4S_, .-_Z3add7Vector4S_
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
