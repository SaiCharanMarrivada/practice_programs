global collatz
section .text

collatz:
    xor eax, eax
    cmp rdi, 1
    je _end

sequence:
    lea rcx, [3 * rdi + 1]
    shr rdi, 1
    cmovc rdi, rcx
    tzcnt rcx, rdi
    lea rax, [rax + rcx + 1]
    shrx rdi, rdi, rcx
    cmp rdi, 1
    jne sequence

_end:
    ret

