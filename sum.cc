#include <immintrin.h>
#include <math.h>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include "bench.h"
#include "hsum.h"

#define  N (int)(1e6)


float sum(float *a, unsigned int n) {
    __m256 partial0 = _mm256_setzero_ps();
    __m256 partial1 = _mm256_setzero_ps();
    float sum = 0.0;
    int i = 0;

    for (i = 0; i < n; i += 16) {
        partial0 = _mm256_add_ps(partial0, _mm256_load_ps(&a[i]));
        partial1 = _mm256_add_ps(partial1, _mm256_load_ps(&a[i + 8]));
    }

    partial0 = _mm256_add_ps(partial0, partial1);

    return hsum(partial0);
}

float sum1(float *a, unsigned int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}


#define SETUP \
    alignas(32) float a[N]; \
    a[0] = 1e5; \
    for (int i = 1; i < N; i++) { \
        a[i] = 1e-5; \
    }

CREATE_BENCHMARK(SETUP, sum, a, N);
CREATE_BENCHMARK(SETUP, sum1, a, N);

BENCHMARK_MAIN();
