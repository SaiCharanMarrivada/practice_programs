#include "benchmark/benchmark.h"

#define CREATE_BENCHMARK(SETUP, function, ...)            \
    void benchmark_##function(benchmark::State &state) {  \
        SETUP;                                            \
        for (auto _ : state) {                            \
            auto y = function(__VA_ARGS__);               \
            benchmark::DoNotOptimize(y);                  \
        }                                                 \
    }                                                     \
    BENCHMARK(benchmark_##function)->Arg(4)
