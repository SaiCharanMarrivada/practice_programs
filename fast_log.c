#include <stdio.h>
#define FORCE_INLINE __attribute__((always_inline)) inline


FORCE_INLINE int ceil_log2(int x) {
    return 32 - __builtin_clz(x - 1);
}

FORCE_INLINE int floor_log2(int x) {
    return 31 - __builtin_clz(x);
}

int main() {
    int x = 5;
    printf("ceil_log2(x) = %d, floor_log2(x) = %d\n",
        ceil_log2(x), floor_log2(x));
}
