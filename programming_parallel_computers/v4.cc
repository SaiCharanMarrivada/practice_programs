#include <cmath>
#include <cstdint>
#include <cstdio>
#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <omp.h>
#include <sys/cdefs.h>
#include <vector>
#include <x86intrin.h>

typedef float Vector8 __attribute__((vector_size(8 * sizeof(float))));
typedef float Vector4 __attribute__((vector_size(4 * sizeof(float))));

const float infinity = std::numeric_limits<float>::infinity();
// zero-cost conversion from `__m256` to `Vector8`
const Vector8 infinity8 = _mm256_set1_ps(infinity); 

void step(float* r, const float* d, int n) {
    // assuming that size is divisible by `blocksize`
    constexpr int vectorsize = 8; // AVX vector
    // std::vector<float> dtransposed(n * n);
    float *dtransposed = (float *)aligned_alloc(32, n * n * sizeof(float));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dtransposed[n*j + i] = d[n*i + j];
        }
    }

#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
asm("innerloop:");
            Vector8 v = infinity8;
            for (int k = 0; k < n; k += vectorsize) {
                Vector8 x = _mm256_load_ps(&d[n*i + k]);
                Vector8 y = _mm256_load_ps(&dtransposed[n*j + k]);
                Vector8 z = x + y;
                v = (v < z) ? v : z;
            }

            Vector4 vlow = _mm256_extractf128_ps(v, 0);
            Vector4 vhigh = _mm256_extractf128_ps(v, 1);
            Vector4 vmin = (vlow < vhigh) ? vlow : vhigh;
            float v1 = std::min(vmin[0], vmin[1]);
            float v2 = std::min(vmin[1], vmin[2]);
            r[n*i + j] = std::min(v1, v2);

asm("innerloop_end:");
        }
    }

}

int main() {
    constexpr int n = 4000;
    // align on 32 byte boundary
    float *d = (float *)aligned_alloc(32, n * n * sizeof(float));
    float *r = (float *)aligned_alloc(32, n * n * sizeof(float));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                d[n*i + j] = 0.0;
            } else {
                d[n*i + j] = (double)(rand() % 30 + 1);
            }
        }
    }

    double t = omp_get_wtime();
    step(r, d, n);
    std::cerr << "time elapsed: " << omp_get_wtime() -  t << "\n";

}
