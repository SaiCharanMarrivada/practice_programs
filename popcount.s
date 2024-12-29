.global popcount

.section .text

//popcount:
//    # not needed as `popcnt` instruction is provided by x86
//    xor %edx, %edx
//    test %rdi, %rdi
//    je _end
//count_ones:
//    lea -1(%rdi), %rax
//    inc %rdx
//    and %rax, %rdi
//    jne count_ones
//_end:
//    mov %rdx, %rax
//    ret

/// repeatedly clear the lsb bit and count
popcount:
    xor %eax, %eax
    test %rdi, %rdi
    je _end
count_ones:
    inc %rax
    blsr %rdi, %rdi
    jne count_ones
_end:
    ret

.section .note.GNU-stack, "", %progbits
