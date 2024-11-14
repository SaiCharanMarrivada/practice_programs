#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <cstdlib>
#include <x86intrin.h>
#include <assert.h>

#define N 100

unsigned int count(float *d, float value, int n) {
    __m256i value8 = (__m256i)_mm256_set1_ps(value);
    int count = 0;

    for (int i = 0; i < n; i += 8) {
        __m256 x = _mm256_load_ps(&d[i]);
        __m256i z = _mm256_cmpeq_epi32((__m256i)x, value8);
        int mask = _mm256_movemask_ps((__m256)z);
        count += __builtin_popcount(mask);
    }

    return count;
}

int main() {
    float d[N];
    for (int i = 0; i < N; i++) {
        d[i] = (float)(rand() % 20);
    }

    for (int i = 0; i < N; i++) {
        // int value = d[i];
        int _count = 0;
        for (int j = 0; j < N; j++) {
            if (d[j] == d[i]) _count++;
        }

        assert(_count == count(d, d[i], N));
    }

}
