#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define  SIZE (1 << 18)
#define BLOCK 1

float sum(float *a, unsigned int n) {
    __m256 partial = _mm256_set1_ps(0.0); // partial sum
    float sum = 0.0;
    int i = 0;

    for (i = 0; i < (8 * BLOCK) * (n / (8 * BLOCK)); i += 8 * BLOCK) {
        for (int j = i; j < i + BLOCK; j++) {
            partial = _mm256_add_ps(partial, _mm256_load_ps(&a[j]));
        }
    }

    for (int j = i; j < n; j++) {
        sum += a[j];
    }


    // horizontal sum
    for (int i = 0; i < 8; i++) {
        sum += partial[i];
    }

    return sum;
}



int main() {
    _Alignas(32) float a[SIZE];
    for (int i = 0; i < SIZE; i++) {
        a[i] = rand() % 100;
    }

    double t = omp_get_wtime();
    printf("%f\n", sum(a, SIZE));
    printf("time elapsed: %f\n", omp_get_wtime() - t);

}
