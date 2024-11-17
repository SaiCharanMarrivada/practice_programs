.globl _start

.section .text

_start:
    mov $33, %edi
    lzcnt %edi, %eax
    mov $31, %edi
    sub %eax, %edi

    mov $60, %eax
    syscall
    ret
