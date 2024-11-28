.global collatz

.section .text


collatz:
    xor %ecx, %ecx
    cmp $1, %rdi
    je _end

sequence:
    lea 1(%rdi, %rdi, 2), %rax
    shr %rdi
    cmovc %rax, %rdi
    inc %rcx
    cmp $1, %rdi
    jne sequence

_end:
    mov %rcx, %rax
    ret

.section .note.GNU-stack, "", %progbits
