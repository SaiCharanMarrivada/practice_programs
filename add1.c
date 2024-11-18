#include <stdio.h>
#include <x86intrin.h>

__m256i add1(__m256i v) {
    return _mm256_sub_epi32(v, _mm256_cmpeq_epi32(v, v));
}

int main() {
    __m256i v = _mm256_set1_epi32(1);
    __m256i y; // add1(v)
    asm("vpcmpeqd %2, %1, %0" : "=x"(y) /* output */ : "x"(v), "x"(v) /* inputs */);
    asm("vpsubd %2, %1, %0" : "=x"(y) : "x"(v), "x"(y));
    unsigned int *a = (unsigned int *)&y;
    for (int i = 0; i < 8; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
