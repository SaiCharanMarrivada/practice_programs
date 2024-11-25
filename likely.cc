#include <stdio.h>
#include <stdlib.h>
#include "bench.h"

#define N (1 << 19)
#define UNLIKELY(x) ((__builtin_expect((x), 0)))
#define NOTHING(x) (x)

#define min(macro, ...)                                 \
    int min_##macro(int *a, size_t size) {              \
        int min = a[0];                                 \
        int index = 0;                                  \
        for (int i = 1; i < (int)size; i++) {           \
            if macro(a[i] < min) {                      \
                min = a[i];                             \
                index = i;                              \
            }                                           \
        }                                               \
        return index;                                   \
    }                                                   \
    CREATE_BENCHMARK(SETUP, min_##macro, __VA_ARGS__)


#define SETUP                     \
    int a[N];                     \
    int sum = 0;                  \
    for (int i = 0; i < N; i++) { \
       a[i] = rand();             \
    }


min(UNLIKELY, a, N);
min(NOTHING, a, N);
BENCHMARK_MAIN();
