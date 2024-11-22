#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include "bench.h"
#include <x86intrin.h>
#include <string.h>

#define N 4096

unsigned int string_length(char *string) {
    unsigned int length = 0;
    // align the string on 32 byte address
    while ((*string) && (((uintptr_t)(string) & (sizeof(__m256) - 1)) != 0)) {
      string++;
      length++;
    }

    if (*string) {
        __m256i zero = _mm256_setzero_si256();
        for(;;) {
#pragma unroll
            for (int i = 0; i < 4; i++) {
                __m256i s = _mm256_load_si256((__m256i *)string);
                __m256i m =_mm256_cmpeq_epi8(s, zero);
                unsigned int mask = _mm256_movemask_epi8(m);
                if (mask) {
                    return length + __builtin_ctz(mask);
                }
                length += 32;
                string += 32;
            }
        }
    }
    return length;
}

#define SETUP                       \
    char test[N];                   \
    for (int i = 0; i < N - 1; i++) \
        test[i] = 'a';              \
    test[N - 1] = '\0'


CREATE_BENCHMARK(SETUP, string_length, test);
CREATE_BENCHMARK(SETUP, strlen, test);

BENCHMARK_MAIN();
