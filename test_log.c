#include <stdio.h>

unsigned int fast_log(unsigned int x); // defined in fast_log.s

int main() {
    printf("log2(32): %d", fast_log(32));
}
