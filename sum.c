#include <immintrin.h>
#include <math.h>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define  SIZE (int)(1e6)

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

    __m128 partial0_low = _mm256_extractf128_ps(partial0, 0);
    __m128 partial1_high = _mm256_extractf128_ps(partial0, 1);
    __m128 partial0_sum = _mm_add_ps(partial0_low, partial1_high);
    partial0_sum = _mm_hadd_ps(partial0_sum, partial0_sum);
    return partial0_sum[0] + partial0_sum[1];
}

float sum1(float *a, unsigned int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}



int main() {
    _Alignas(32) float a[SIZE];
    a[0] = 1e5;
    for (int i = 1; i < SIZE; i++) {
        a[i] = 1e-5;
    }

    double t = omp_get_wtime();
    printf("%f\n", sum(a, SIZE));
    printf("time elapsed: %f\n", omp_get_wtime() - t);

}
