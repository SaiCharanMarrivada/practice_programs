#include <cstdio>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <omp.h>

void step(float* r, const float* d, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float v = std::numeric_limits<float>::infinity();
            for (int k = 0; k < n; ++k) {
                float x = d[n*i + k];
                float y = d[n*k + j];
                float z = x + y;
                v = std::min(v, z);
            }
            r[n*i + j] = v;
        }
    }
}

int main() {
    constexpr int n = 2000;
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

    free(d);
    free(r);
}
