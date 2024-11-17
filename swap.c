#include <stdio.h>

int main() {
    int x = 1;
    int y = 2;
    int temp;
    asm("mov %1, %0" : "=r"(temp) : "r"(y));
    asm("mov %1, %0" : "=r"(y) : "r"(x));
    asm("mov %1, %0" : "=r"(x) : "r"(temp));
    printf("x = %d, y = %d\n", x, y);
}
