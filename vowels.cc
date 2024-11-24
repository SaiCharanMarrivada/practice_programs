#include <string.h>
#include <x86intrin.h>
#include <stdint.h>
#include <stdio.h>
#include "bench.h"

#define UNLIKELY(x) (__builtin_expect((x), 0))

#define N 100002

int vowels(char *string) {
    char vowels[] = "aeiou";
    int count = 0;
    while (*string) {
        /*
        if (memchr(vowels, *string, 5)) {
            count++;
        }
        */
        if (*string == 'a') count++;
        if (*string == 'e') count++;
        if (*string == 'i') count++;
        if (*string == 'o') count++;
        if (*string == 'u') count++;
        string++;
    }
    return count;
}

int vowels2(char *string) {
    char vowels[] = "aeiou";
    int count = 0;
    while (*string && ((uintptr_t)(string) & 31)) {
        if (memchr(vowels, *string, 5)) {
            count++;
        }
        string++;
    }

    if (*string) {
        __m256i a8 = _mm256_set1_epi8('a');
        __m256i e8 = _mm256_set1_epi8('e');
        __m256i i8 = _mm256_set1_epi8('i');
        __m256i o8 = _mm256_set1_epi8('o');
        __m256i u8 = _mm256_set1_epi8('u');
        __m256i count8 = _mm256_setzero_si256();
        __m256i zero8 = _mm256_setzero_si256();

        for (;;) {
            __m256i s8 = _mm256_load_si256((__m256i *)string);
            count8 = _mm256_setzero_si256();
            count8 = _mm256_add_epi8(count8, _mm256_cmpeq_epi8(s8, a8));
            count8 = _mm256_add_epi8(count8, _mm256_cmpeq_epi8(s8, e8));
            count8 = _mm256_add_epi8(count8, _mm256_cmpeq_epi8(s8, i8));
            count8 = _mm256_add_epi8(count8, _mm256_cmpeq_epi8(s8, o8));
            count8 = _mm256_add_epi8(count8, _mm256_cmpeq_epi8(s8, u8));
            count += __builtin_popcount(_mm256_movemask_epi8(count8));
            __m256i m = _mm256_cmpeq_epi8(zero8, s8);
            if UNLIKELY(!_mm256_testz_si256(m, m)) {
                return count;
            }
            string += 32;
        }

    }

    return count;


}

#define SETUP                         \
    char string[N];                   \
    for (int i = 0; i < N - 1; i++) { \
        string[i] = 'a';              \
    }                                 \
    string[N - 1] = '\0'

CREATE_BENCHMARK(SETUP, vowels, string);
CREATE_BENCHMARK(SETUP, vowels2, string);

BENCHMARK_MAIN();
