#include <emmintrin.h>
#include <immintrin.h>
#include "bench.h"
#include <x86intrin.h>
#include <assert.h>

#define N (int)4e5
typedef int Vector8 __attribute__((vector_size(32)));

unsigned int count(float *d, float value, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (d[i] == value) count++;
    }

    return count;
}

int count1(float *d, float value, int n) {
    __m256i value8 = (__m256i)_mm256_set1_ps(value);
    __m256i s1 = _mm256_setzero_si256();
    __m256i s2 = _mm256_setzero_si256();

    for (int i = 0; i < n; i += 16) {
        __m256i y1 = (__m256i)_mm256_load_ps(&d[i]);
        __m256i y2 = (__m256i)_mm256_load_ps(&d[i + 8]);
        __m256i mask1 = _mm256_cmpeq_epi32(value8, y1);
        __m256i mask2 = _mm256_cmpeq_epi32(value8, y2);
        s1 = _mm256_add_epi32(s1, mask1);
        s2 = _mm256_add_epi32(s2, mask2);
    }
    s1 = _mm256_add_epi32(s1, s2);
    // Vector8 s1v = Vector8(s1);
    int *v = (int *)&s1;
    int sum = 0;
    for (int i = 0; i < 8; i++) sum += v[i];
    return -sum;
}

#define SETUP                   \
    float d[N];                 \
    for (int i = 0; i < N; i++) \
        d[i] = rand() % 100

CREATE_BENCHMARK(SETUP, count, d, 11, N);
CREATE_BENCHMARK(SETUP, count1, d, 11, N);

BENCHMARK_MAIN();
