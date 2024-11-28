.global collatz

.section .text


collatz:
    xor %eax, %eax
    cmp $1, %rdi
    je _end

sequence:
    lea 1(%rdi, %rdi, 2), %rcx
    # inc %rax
    shr %rdi
    cmovc %rcx, %rdi
    inc %rax
    # cmp $1, %rdi
    tzcnt %rdi, %rcx
    add %rcx, %rax
    shr %cl, %rdi
    cmp $1, %rdi
    jne sequence

_end:
    ret

.section .note.GNU-stack, "", %progbits
