#include <stdio.h>
#define NOINLINE __attribute__((noinline))

NOINLINE void swap(int *x, int *y) {
    /*
    int temp = *x;
    *x = *y;
    *y = temp;
    */
    __asm__(
        "mov (%rdi), %eax \n\t"
        "mov (%rsi), %edx \n\t"
        "mov %edx, (%rsi) \n\t"
        "mov %eax, (%rdi) "
    );
}

int main() {
    int x = 1;
    int y = 2;
    swap(&x, &y);
    printf("x = %d, y = %d\n", x, y);
}
