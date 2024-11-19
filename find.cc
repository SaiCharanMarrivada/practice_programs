#include <stdlib.h>
#include <assert.h>
#include "benchmark/benchmark.h"
#include <algorithm>
#include <x86intrin.h>
#define N 4000

int find(int *a, int item) {
    __m256i item8 = _mm256_set1_epi32(item);
    for (int i = 0; i < N; i += 8) {
        __m256i v = _mm256_load_si256((__m256i *)&a[i]);
        __m256i m = _mm256_cmpeq_epi32(item8, v);
        if (!_mm256_testz_si256(m, m)) {
            return i + __builtin_ctz(_mm256_movemask_ps((__m256)m));
        }
    }
    return -1;
}

int find1(int *a, int item) {
    for (int i = 0; i < N; i++) {
        if (a[i] == item) return i;
    }
    return -1;
}

int find2(int *a, int item) {
    return std::lower_bound(a, a + N, item) - a;
}

#define CREATE_BENCHMARK(function, n)                      \
    void benchmark_##function(benchmark::State &state) {   \
        int a[n];                                          \
        for (int i = 0; i < n; i++) {                      \
            a[i] = rand() % 100;                           \
        }                                                  \
        std::sort(a, a + n);                               \
        for (auto _ : state) {                             \
            int index = function(a, 102);                  \
            benchmark::DoNotOptimize(index);               \
        }                                                  \
    }

CREATE_BENCHMARK(find, N)
CREATE_BENCHMARK(find1, N)
CREATE_BENCHMARK(find2, N)

BENCHMARK(benchmark_find1)->Arg(4);
BENCHMARK(benchmark_find)->Arg(4);
BENCHMARK(benchmark_find2)->Arg(4);

BENCHMARK_MAIN();
