#include "bench.h"
#include <string.h>

float inverse_sqrt(float x) {
    float y;
    asm("vrsqrtss %1, %1, %0" : "=x"(y) : "x"(x));
    return y;
}

float inverse_sqrt2(float x) {
    // Quake-III algorithm
    float y;
    unsigned int i;
    memcpy(&i, &x, sizeof(float));
    i = 0x5f3759df - (i >> 1);
    // initial approximation for `inverse_sqrt(x)`
    memcpy(&y, &i, sizeof(float));
    // first iteration of newton's method
    y = y * (1.5f - (0.5 * x * y * y));
    return y;
}

#define SETUP                   \
    float x = 2;                \
    benchmark::DoNotOptimize(x) \


CREATE_BENCHMARK(SETUP, inverse_sqrt, x);
CREATE_BENCHMARK(SETUP, inverse_sqrt2, x);

BENCHMARK_MAIN();
