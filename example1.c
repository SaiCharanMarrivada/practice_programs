#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define SIZE (1L << 16)

void test(uint8_t *restrict a, uint8_t *restrict b) {
    // restrict tells the compiler that no overlap is
    // possible
    // alignment for SSE: 16 bytes
    // alignment for AVX2: 32 bytes
    // a = __builtin_assume_aligned(a, 16);
    // b = __builtin_assume_aligned(b, 16);
    a = __builtin_assume_aligned(a, 32);
    b = __builtin_assume_aligned(b, 32);

    for (int i = 0; i < SIZE; i++) {
        a[i] += b[i];
    }
}
