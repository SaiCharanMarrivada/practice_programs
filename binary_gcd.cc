#include <math.h>
#include <stdlib.h>
#include "bench.h"
#include <numeric>
#include <algorithm>

using std::gcd;


int binary_gcd(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;

    int az = __builtin_ctz(a);
    int bz = __builtin_ctz(b);
    int shift = std::min(az, bz);
    a >>= az;
    b >>= bz;

    while (a != 0) {
        int difference = a - b;
        int b = std::min(a, b);
        int a = std::abs(difference);
        a >>= __builtin_ctz(a);
    }

    return b << shift;
}


#define SETUP

CREATE_BENCHMARK(SETUP, binary_gcd, rand(), rand());
CREATE_BENCHMARK(SETUP, gcd, rand(), rand());

BENCHMARK_MAIN();
