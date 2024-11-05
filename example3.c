#include <stdint.h>
#define  SIZE (1L << 16)

void test(uint8_t *restrict a, uint8_t *restrict b) {
    // the below loop is optimized to __builtin_memcpy(a, b + 1, SIZE)
    for (int i = 0; i < SIZE; i++) {
        a[i] = b[i + 1];
    }
}
