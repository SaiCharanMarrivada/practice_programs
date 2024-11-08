#include <stdio.h>
#define NOINLINE __attribute__((noinline))

NOINLINE unsigned int ceil_log2(unsigned int n) {
    __asm__ (
        "sub r0, r0, #1 \n"
        "clz r0, r0 \n"
        "rsb r0, r0, #32 \n"
    );
}

int main() {
    printf("ceil_log2(100) = %u\n", ceil_log2(100));
}
