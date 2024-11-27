.global fast_log

.section .text

fast_log:
    lzcnt %edi, %edi
    mov $31, %eax
    sub %edi, %eax
    ret

.section .note.GNU-stack, "" , %progbits
