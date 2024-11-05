#ifndef SIMD_H
#define SIMD_H

#include <immintrin.h>

#include <memory>
#define FORCE_INLINE __attribute__((always_inline)) inline

using float32x8 = __m256;
using int32x8 = __m256i;

namespace simd {

FORCE_INLINE float32x8 load_value(float f) {
    return _mm256_set1_ps(f);
}

FORCE_INLINE int32x8 load_value(int i) {
    return _mm256_set1_epi32(i);
}

FORCE_INLINE float32x8 load_from(float *f) {
    return _mm256_loadu_ps(f); // unaligned load
}

FORCE_INLINE int32x8 load_from(int *i) {
    return _mm256_loadu_epi32(i);
}

FORCE_INLINE float32x8 masked_load_from(float *f, float fill, int mask) {
    return _mm256_mask_loadu_ps(_mm256_set1_ps(fill), (__mmask8)mask, f);
}

FORCE_INLINE float32x8 masked_load_from(float *f, float32x8 fill, int mask) {
    return _mm256_mask_loadu_ps(fill, (__mmask8)mask, f);
}

FORCE_INLINE void masked_store_to(float32x8 dest, float *f, int mask) {
    _mm256_mask_storeu_ps(f, (__mmask8)mask, dest);
}

FORCE_INLINE void store_to(float32x8 dest, float *f) {
    masked_store_to(dest, f, 255);
}

template <unsigned A = 0, unsigned B = 0, unsigned C = 0, unsigned D = 0, 
          unsigned E = 0, unsigned F = 0, unsigned G = 0, unsigned H = 0>
constexpr int make_bitmask() {
    return ((A << 0) | (B << 1) | (C << 2) | (D << 3) | 
            (E << 4) | (F << 5) | (G << 6) | (H << 7));
}

FORCE_INLINE float32x8 blend(float32x8 a, float32x8 b, int mask) {
    return _mm256_mask_blend_ps((__mmask8)mask, a, b);
}

FORCE_INLINE float32x8 masked_permute(float32x8 a, float32x8 b, 
	int32x8 permutation, int mask) {
    return _mm256_mask_permutexvar_ps(a, (__mmask8)mask, permutation, b);
}

FORCE_INLINE float32x8 permute(float32x8 a, int32x8 permutation) {
    return _mm256_permutexvar_ps(permutation, a);
}

template <unsigned A, unsigned B, unsigned C, unsigned D,
          unsigned E, unsigned F, unsigned G, unsigned H>
constexpr int32x8 make_permutation() {
    return _mm256_setr_epi32(A, B, C, D, E, F, G, H);
}

template <int R>
float32x8 rotate(float32x8 r) {
    if (R % 8 == 0) {
        return r;
    }

    int S = (R > 0) ? 8 - R % 8 : -R;
    int A = (S) % 8;
    int B = (S + 1) % 8;
    int C = (S + 2) % 8;
    int D = (S + 3) % 8;
    int E = (S + 4) % 8;
    int F = (S + 5) % 8;
    int G = (S + 6) % 8;
    int H = (S + 7) % 8;
    return _mm256_permutexvar_ps(_mm256_setr_epi32(A, B, C, D, E, F, G, H), r);
}

template <int R>
float32x8 rotate_right(float32x8 r) {
    return rotate<-R>(r);
}

template<int S>
__mmask8 shift_blend_mask() {
    if (S < 0) return (1 << (-S % 8)) - 1;
    return ~((1 << ((8 - S % 8))) - 1);
}

template <int S>
float32x8 shift_right(float32x8 s) {
    return blend(rotate_right<S>(s), load_value((float)0.0), shift_blend_mask<S>());
}

template <int S>
float32x8 shift_right_with_carry(float32x8 a, float32x8 b) {
    return blend(rotate_right<S>(a), rotate_right<S>(b), shift_blend_mask<S>());
}

template <int S>
float32x8 shift_left(float32x8 s) {
    return blend(rotate<S>(s), load_value((float)0.0), shift_blend_mask<-S>());
}

template <int S>
float32x8 shift_left_with_carry(float32x8 a, float32x8 b) {
    return blend(rotate<S>(a), rotate<S>(b), shift_blend_mask<-S>());
}

FORCE_INLINE float32x8 fma(float32x8 a, float32x8 b, float32x8 c) {
    return _mm256_fmadd_ps(a, b, c);
}

FORCE_INLINE float32x8 minimum(float32x8 a, float32x8 b) {
    return _mm256_min_ps(a, b);
}

FORCE_INLINE float32x8 maximum(float32x8 a, float32x8 b) {
    return _mm256_max_ps(a, b);
}

FORCE_INLINE float32x8 compare_with_exchange(float32x8 values, 
        int32x8 permutation, int mask) {
    // `permutation` specifies which ones to be compared
    // `mask` specifies when 2 elements are compared which
    // one is the maximum
    auto exchange = permute(values, permutation);
    auto vmin = minimum(exchange, values);
    auto vmax = maximum(exchange, values);
    return blend(vmin, vmax, mask);
}


FORCE_INLINE float32x8 sorting_network(float32x8 input) {
    // batcher's sorting network for n=8
    auto permutation1 = make_permutation<1, 0, 3, 2, 5, 4, 7, 6>();
    auto mask1 = make_bitmask<0, 1, 0, 1, 0, 1, 0, 1>();

    auto permutation2 = make_permutation<3, 2, 1, 0, 7, 6, 5, 4>();
    auto mask2 = make_bitmask<0, 0, 1, 1, 0, 0, 1, 1>();

    auto permutation3 = make_permutation<1, 0, 3, 2, 5, 4, 7, 6>();
    auto mask3 = make_bitmask<0, 1, 0, 1, 0, 1, 0, 1>();

    auto permutation4 = make_permutation<7, 6, 5, 4, 3, 2, 1, 0>();
    auto mask4 = make_bitmask<0, 0, 0, 0, 1, 1, 1, 1>();

    auto permutation5 = make_permutation<2, 3, 0, 1, 6, 7, 4, 5>();
    auto mask5 = make_bitmask<0, 0, 1, 1, 0, 0, 1, 1>();

    auto permutation6 = make_permutation<1, 0, 3, 2, 5, 4, 7, 6>();
    auto mask6 = make_bitmask<0, 1, 0, 1, 0, 1, 0, 1>();

    input = compare_with_exchange(input, permutation1, mask1);
    input = compare_with_exchange(input, permutation2, mask2);
    input = compare_with_exchange(input, permutation3, mask3);
    input = compare_with_exchange(input, permutation4, mask4);
    input = compare_with_exchange(input, permutation5, mask5);
    input = compare_with_exchange(input, permutation6, mask6);

    return input;

}


} // namespace simd
#endif // SIMD_H
