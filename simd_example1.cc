#include <immintrin.h>
#include <x86intrin.h>

int main() {
    double a[100], b[100], c[100];

    for (int i = 0; i < 100; i += 4) {
        // aligned loads
        __m256d x = _mm256_load_pd(&a[i]);
        __m256d y = _mm256_load_pd(&b[i]);

        __m256d z = _mm256_add_pd(x, y);
        // aligned store
        _mm256_store_pd(&c[i], z);
    }
}
