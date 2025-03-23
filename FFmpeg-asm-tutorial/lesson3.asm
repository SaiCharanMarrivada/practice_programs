; void add_values(uint8_t *source, uint8_t *target, ptrdiff_t width)

section .text

global add_values

add_values:
    add rdi, rdx
    add rsi, rdx
    neg rdx

addvalues:
    movdqu xmm0, [rdi]
    movdqu xmm1, [rsi]
    paddb xmm0, xmm1
    movdqu [rdi], xmm0
    add rdx, 16
    jl addvalues
    ret
