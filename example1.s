	.text
	.file	"example1.c"
	.globl	test                            # -- Begin function test
	.p2align	4, 0x90
	.type	test,@function
test:                                   # @test
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	.p2align	4, 0x90
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	vmovdqa	(%rdi,%rax), %ymm0
	vmovdqa	32(%rdi,%rax), %ymm1
	vmovdqa	64(%rdi,%rax), %ymm2
	vmovdqa	96(%rdi,%rax), %ymm3
	vpaddb	(%rsi,%rax), %ymm0, %ymm0
	vpaddb	32(%rsi,%rax), %ymm1, %ymm1
	vpaddb	64(%rsi,%rax), %ymm2, %ymm2
	vpaddb	96(%rsi,%rax), %ymm3, %ymm3
	vmovdqa	%ymm0, (%rdi,%rax)
	vmovdqa	%ymm1, 32(%rdi,%rax)
	vmovdqa	%ymm2, 64(%rdi,%rax)
	vmovdqa	%ymm3, 96(%rdi,%rax)
	vmovdqa	128(%rdi,%rax), %ymm0
	vmovdqa	160(%rdi,%rax), %ymm1
	vmovdqa	192(%rdi,%rax), %ymm2
	vmovdqa	224(%rdi,%rax), %ymm3
	vpaddb	128(%rsi,%rax), %ymm0, %ymm0
	vpaddb	160(%rsi,%rax), %ymm1, %ymm1
	vpaddb	192(%rsi,%rax), %ymm2, %ymm2
	vpaddb	224(%rsi,%rax), %ymm3, %ymm3
	vmovdqa	%ymm0, 128(%rdi,%rax)
	vmovdqa	%ymm1, 160(%rdi,%rax)
	vmovdqa	%ymm2, 192(%rdi,%rax)
	vmovdqa	%ymm3, 224(%rdi,%rax)
	addq	$256, %rax                      # imm = 0x100
	cmpq	$65536, %rax                    # imm = 0x10000
	jne	.LBB0_1
# %bb.2:
	vzeroupper
	retq
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
	.cfi_endproc
                                        # -- End function
	.ident	"Ubuntu clang version 14.0.0-1ubuntu1.1"
	.section	".note.GNU-stack","",@progbits
	.addrsig
