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
    auto a_vector = (__m256i *)a;
    auto b_vector = (__m256i *)b;
    for (int i = 0; i < N / 32; i += 2) {
        for (int j = i; j < i + 2; j++) {
            auto a8 = _mm256_load_si256(&a_vector[j]);
            auto b8 = _mm256_load_si256(&b_vector[j]);
            auto max8 = _mm256_max_epu8(a8, b8);
            _mm256_store_si256(&a_vector[j], max8);
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
