.global collatz

.section .text

collatz:
    xor %eax, %eax
    cmp $1, %rdi
    je _end

sequence:
    lea 1(%rdi, %rdi, 2), %rcx
    shr %rdi
    cmovc %rcx, %rdi
    tzcnt %rdi, %rcx
    lea 1(%rax, %rcx), %rax 
    shrx %rcx, %rdi, %rdi
    cmp $1, %rdi
    jne sequence

_end:
    ret

.section .note.GNU-stack, "", %progbits
