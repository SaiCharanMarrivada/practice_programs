global shuffle_mask

section .rodata

mask:
    db 4, 3, 1, 2, -1, 2, 3, 7, 5, 4, 3, 8, 12, 13, 15, -1

section .text
shuffle_mask:
    movdqu xmm0, [rdi]
    movdqu xmm1, [rel mask]
    pshufb xmm0, xmm1
    movdqu [rdi], xmm0
    ret
