#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <omp.h>
#include <vector>

void step(float* r, const float* d, int n) {
    // assuming that size is divisible by `blocksize`
    constexpr int blocksize = 8;
    const float infinity = std::numeric_limits<float>::infinity();
    std::vector<float> dtransposed(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dtransposed[n*j + i] = d[n*i + j];
        }
    }

#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
asm("innerloop:");
            float v[blocksize];
            for (int i = 0; i < blocksize; i++) v[i] = infinity;
            for (int k = 0; k < n; k += blocksize) {
                for (int l = k; l < k + blocksize; l++) {
                    float x = d[n*i + l];
                    float y = dtransposed[n*j + l];
                    v[l] = std::min(x + y, v[l]);
                }
            }

            float v1 = infinity;
            for (int i = 0; i < blocksize; i++) {
                v1 = std::min(v[i], v1);
            }
            r[n*i + j] = v1;
asm("innerloop_end:");
        }
    }

}

int main() {
    constexpr int n = 4000;
    float *d = (float *)malloc(n * n * sizeof(float));
    float *r = (float *)malloc(n * n * sizeof(float));

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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << r[n*i + j] << " ";
        }
        std::cout << "\n";
    }


}
