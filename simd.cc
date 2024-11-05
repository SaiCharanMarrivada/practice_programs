#include "simd.h"
#include <cstdio>
#include <stdio.h>

int main() {
    float32x8 input = {3, 2, 1, 0, 5, 6, 8, 7};
    float32x8 output = simd::sorting_network(input);

    for (int i = 0; i < 7; i++) {
        printf("%f ", output[i]);
    }
}
