#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE (1 << 16)
#define LIKELY(x) ((__builtin_expect((x), 1)))

int get(int *a, size_t index, size_t size) {
    if LIKELY(index < size) {
        return a[index];
    }
    return a[index % size];
}

int main() {
    int a[SIZE];
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
       a[i] = rand() % 200;
    }

    double t = omp_get_wtime();
    for (int i = 0; i < SIZE; i++) {
        sum += get(a, i, SIZE);
    }

    printf("%d\n", sum);
    printf("time elapsed: %f\n", omp_get_wtime() - t);
}
