#include <immintrin.h>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define  SIZE (int)(1e5)

float sum(float *a, unsigned int n) {
    __m256 partial = _mm256_set1_ps(0.0); // partial sum
    float sum = 0.0;
    int i = 0;

    for (i = 0; i < n; i += 8) {
        partial = _mm256_add_ps(partial, _mm256_load_ps(&a[i]));
    }
    // horizontal sum
    for (int i = 0; i < 8; i++) {
        sum += partial[i];
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
    // non-simd sum gives 10^5!! simd-sum gives 10^5 + 1 which is correct
    printf("%f\n", sum(a, SIZE));
    printf("time elapsed: %f\n", omp_get_wtime() - t);

}
