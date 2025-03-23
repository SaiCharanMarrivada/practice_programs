global signextend

section .text

signextend:
    pxor xmm0, xmm0
    movdqu xmm1, [rdi]
    movdqu xmm2, xmm1
    pcmpgtb xmm0, xmm1
    punpcklbw xmm1, xmm0
    punpckhbw xmm2, xmm0
    movdqu [rsi], xmm1
    movdqu [rsi + 16], xmm2
    ret
