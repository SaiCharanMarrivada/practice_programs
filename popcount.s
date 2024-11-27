.global popcount

.section .text

popcount:
    # not needed as `popcnt` instruction is provided by x86
    xor %edx, %edx
    test %rdi, %rdi
    je _end
count_ones:
    lea -1(%rdi), %rax
    inc %rdx
    and %rax, %rdi
    jne count_ones
_end:
    mov %rdx, %rax
    ret

.section .note.GNU-stack, "", %progbits
