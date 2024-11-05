#include "simd.h"
#include <cstdio>
#include <cassert>

int main() {
    float32x8 input = {2, 3, 4, 7, 1, 0, 6, 5}; 

    input = simd::sorting_network(input);

    float32x8 output = simd::shift_right<3>(input);
    float32x8 output1 = simd::shift_right<11>(input);

    for (int i = 0; i < 8; i++) {
        assert(output[i] == output1[i]);
    }
}

