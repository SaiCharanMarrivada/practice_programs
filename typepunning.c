#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <string.h>
#define NOINLINE __attribute__((noinline))

float typepunning_memcpy(int x) {
    float y;
    memcpy(&y, &x, sizeof(float));
    return y;
}

float typepunning_union(int x) {
    typedef struct {
        union {
            float float32;
            int int32;
        } as;
    } Bits;

    Bits y = {.as.int32 = x};
    return y.as.float32;
}

float typepunning_bitcasting(int x) {
    return *(float *)&x;
}


int main() {
    int x = 0x80000000; // -0.0
    float y;
    asm("vmovd %0, %%xmm0" : "=r"(y) : "r"(x)); // gnu inline asm
    printf("%f\n", y);
}
