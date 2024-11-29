#include <cstdint>
#include "bench.h"
#include <stdint.h>

extern "C" {
uint64_t collatz(uint64_t n);
}

uint64_t collatz2(uint64_t n) {
    uint64_t count = 0;
    while (n != 1) {
        if (n % 2 != 0) {
            n = 3 * n + 1;
        } else {
            n /= 2;
        }
        auto shift = __builtin_ctz(n);
        n >>= shift;
        count += shift;
        count++;
    }
    return count;
}

#define SETUP uint64_t n = 190000

CREATE_BENCHMARK(SETUP, collatz, n);
CREATE_BENCHMARK(SETUP, collatz2, n);

BENCHMARK_MAIN();
