#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool same_set(unsigned int address1, unsigned int address2) {

    // cache of size `12KB`, 3-way associative, 256 sets
    // returns `true` is 2 addresses map to same cache
    unsigned int mask = ((1 << 8) - 1) << 4;
    // compiler optimizes this to `(address1 ^ address2) & mask`
    return (address1 & mask) == (address2 & mask);
}

int main() {
    unsigned int address1 = 0x12345678;
    unsigned int address2 = 0x87654321;
    printf("same_set(0x%x, 0x%x) = %d\n", address1, address2,
        same_set(address1, address2));
}
