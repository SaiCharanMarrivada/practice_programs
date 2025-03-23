section .text

global add_values

%if 0
the below code is equivalent to the following C code
    // `restrict` is to tell the compiler that there
    // is no aliasing
    void add_values(uint8_t *restrict source, uint8_t *target) {
        for (int i = 0; i < 16; i++) {
            source[i] += target[i];
        }
    }
%endif

add_values:
    ; loads 4 values from first argument into `xmm0`
    movdqu xmm0, [rdi]
    ; loads 4 values from second argument into `xmm1`
    movdqu xmm1, [rsi]
    ; add `xmm0` and `xmm1` and store the result in `xmm0`
    paddb xmm0, xmm1
    ; store the result in `xmm0` into first argument(address)
    movdqu [rdi], xmm0
    ret
