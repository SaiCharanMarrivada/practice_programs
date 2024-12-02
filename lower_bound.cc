#include <algorithm>
#include <stdlib.h>
#include "bench.h"
#define N (1 << 24)

using std::lower_bound;

int _lower_bound(int *a, int x, unsigned int n) {
    int *current = a;
    unsigned int length = n;
    while (length > 1) {
        unsigned int half_length = length / 2;
        __builtin_prefetch(&a[half_length - 1]);
        __builtin_prefetch(&a[length - 1]);
        current += (a[half_length - 1] < x) * half_length;
        length -= half_length;
    }
    return *current;
}

#define SETUP   \
    int *a = (int *)malloc(N * sizeof(int));   \
    for (int i = 0; i < N; i++) { \
        a[i] = rand() % 100; \
    } \
    int x = 101;

CREATE_BENCHMARK(SETUP, _lower_bound, a, x, N);
CREATE_BENCHMARK(SETUP, lower_bound, a, a + N, x);

BENCHMARK_MAIN();
