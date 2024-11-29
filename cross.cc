#include <x86intrin.h>
#include "bench.h"

__m128 cross(__m128 v1, __m128 v2) {
    const auto permutation1 = _MM_SHUFFLE(3 /*z*/, 0 /*x*/, 2 /*y*/, 1 /*w*/);
    const auto permutation2 = _MM_SHUFFLE(3, 1, 0, 2);
    auto p1 = _mm_shuffle_ps(v1, v1, permutation1);
    auto p2 = _mm_shuffle_ps(v2, v2, permutation2);
    auto s1 = _mm_mul_ps(p1, p2);

    auto p3 = _mm_shuffle_ps(v1, v1, permutation2);
    auto p4 = _mm_shuffle_ps(v2, v2, permutation1);
    auto s2 = _mm_mul_ps(p3, p4);

    return _mm_sub_ps(s2, s1);
}

struct Vector4 {
    float v[4];

    float &operator[](int i) {
        return v[i];
    }
};

Vector4 cross2(Vector4 v1, Vector4 v2) {
    Vector4 v;
    v[0] = v1[1] * v2[2] - v2[1] * v1[2];
    v[1] = v1[2] * v2[0] - v1[0] * v2[2];
    v[2] = v1[0] * v2[1] - v1[1] * v2[0];
    // v[3] = 0;
    return v;
}


#define SETUP(type)                             \
    type v1 = {rand(), rand(), rand(), rand()}; \
    type v2 = {rand(), rand(), rand(), rand()}

CREATE_BENCHMARK(SETUP(__m128), cross, v1, v2);
CREATE_BENCHMARK(SETUP(Vector4), cross2, v1, v2);
BENCHMARK_MAIN();
