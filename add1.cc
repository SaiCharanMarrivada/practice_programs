#include <stdio.h>
#include "benchmark/benchmark.h"
#include <x86intrin.h>
#define FORCE_INLINE __attribute__((always_inline)) inline

FORCE_INLINE __m256i add1(__m256i v) {
    return _mm256_sub_epi32(v, _mm256_cmpeq_epi32(v, v));
}

void benchmark_add1(benchmark::State &state) {
    for (auto _ : state) {
        __m256i v = _mm256_set1_epi32(1);
        benchmark::DoNotOptimize(v);
        __m256i v1 = add1(v);
        benchmark::DoNotOptimize(v1);
    }
}

void benchmark_asm(benchmark::State &state) {
    for (auto _ : state) {
        __m256i v = _mm256_set1_epi32(1);
        benchmark::DoNotOptimize(v);
        __m256i y;
        asm("vpcmpeqb %1, %1, %0" : "=x"(y) : "x"(v));
        asm("vpsubd %2, %1, %0" : "=x"(y) : "x"(v), "x"(y));
        benchmark::DoNotOptimize(y);
    }
}

BENCHMARK(benchmark_asm)->Arg(4);
BENCHMARK(benchmark_add1)->Arg(4);
BENCHMARK_MAIN();
