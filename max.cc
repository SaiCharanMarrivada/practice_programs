#include "bench.h"
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include <x86intrin.h>
#include <xmmintrin.h>

#define N (1 << 14)

uint8_t max1(uint8_t *a, uint8_t *b) {
    for (int i = 0; i < N; i++) {
        a[i] = std::max(a[i], b[i]);
    }
    return a[rand() % N];
}

uint8_t max2(uint8_t *a, uint8_t *b) {
    for (int i = 0; i < N; i += 128) {
        for (int j = 0; j < 4; j++) {
            auto a8 = _mm256_load_si256((__m256i *)&a[i]);
            auto b8 = _mm256_load_si256((__m256i *)&b[i]);
            auto max8 = _mm256_max_epu8(a8, b8);
            _mm256_store_si256((__m256i *)&a[i], max8);
        }
    }
    return a[rand() % N];
}

#define SETUP     \
    alignas(32) uint8_t a[N]; \
    alignas(32) uint8_t b[N]; \
    for (int i = 0; i < N; i++) { \
        a[i] = rand() % 256; \
        b[i] = rand() % 256; \
    }

CREATE_BENCHMARK(SETUP, max1, a, b);
CREATE_BENCHMARK(SETUP, max2, a, b);

BENCHMARK_MAIN();
