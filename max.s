.globl _start

.section .data


numbers:
    .quad 1, 2, 3, 10, 5, 6, 17
numbers_end:

.equ no_of_numbers, (numbers_end - numbers) / 8

.section .text

_start:
    movq $no_of_numbers, %rcx
    movq $0, %rdi

    cmpq $0, %rcx
    je _end

_max:
    movq numbers-8(, %rcx, 8), %rax
    cmpq %rdi, %rax
    cmovaq %rax, %rdi # conditional move
    loopq _max

_end:
    movq $60, %rax
    syscall

