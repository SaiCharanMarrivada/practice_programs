#include <stdint.h>

#define SIZE (1L << 16)

void test(uint8_t *restrict a, uint8_t *restrict b) {
    a = __builtin_assume_aligned(a, 32);
    b = __builtin_assume_aligned(b, 32);

    for (int i = 0; i < SIZE; i++) {
        a[i] = (b[i] > a[i]) ? b[i] : a[i];
    }
}
