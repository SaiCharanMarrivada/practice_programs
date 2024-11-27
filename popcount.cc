#include <stdio.h>
#include "bench.h"
#include <x86intrin.h>
#include <stdint.h>

extern "C" {
uint64_t popcount(uint64_t n); // defined in popcount.s
}

#define SETUP uint64_t n = -1

CREATE_BENCHMARK(SETUP, popcount, n);
CREATE_BENCHMARK(SETUP, _mm_popcnt_u64, n);
BENCHMARK_MAIN();
