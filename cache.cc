#include <iostream>

// cache is 12kB size, 3-way set associative, 256 sets

template <unsigned int N, unsigned int M = 0>
struct BitMask {
    constexpr static unsigned int value = ((1 << N) - 1) << M;
};

bool same_set(unsigned int address1, unsigned int address2) {
    return (address1 ^ address2) & BitMask<8, 4>::value;
}

bool same_block(unsigned int address1, unsigned int address2) {
    return ((address1 ^ address2) >> 4) == 0;
}

int main() {
    unsigned int address1 = 0x12345678;
    unsigned int address2 = 0x87654321;

    std::cout << same_set(address1, address2) << "\n";
    std::cout << same_block(address1, address2) << "\n";
}
