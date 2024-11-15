#include <emmintrin.h>
#include <immintrin.h>
#include <stdio.h>
#include <x86intrin.h>
#include <assert.h>

#define N 400
typedef int Vector8 __attribute__((vector_size(32)));

#ifdef VERSION1
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
#else
int count(float *d, float value, int n) {
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
    Vector8 s1v = Vector8(s1);
    int sum = 0;
    for (int i = 0; i < 8; i++) sum += s1v[i];
    return -sum;
}

#endif

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

        if (_count != count(d, d[i], N)) {
            printf("_count = %d, count(d, d[%d], %d) = %u\n", _count, i, N,
                count(d, d[i], N));
        }
    }

}
