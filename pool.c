#include <stdio.h>
#include <assert.h>

char bytes[4096]; // small pool
char *current = &bytes[0];

void *allocate(int size) {
    if (current + size < &bytes[0] + 4096) {
        current += size;
        return current - size;
    }
    return NULL;
}

int main() {
    int sum = 0;
#pragma omp parallel for
    for (int i = 0; i < 12; i++) {
        int temp_sum = 0;
        int *a;
#pragma omp critical
        a = (int *)allocate(10 * sizeof(int));
        for (int i = 0; i < 10; i++) {
            a[i] = i;
            temp_sum += a[i];
        }
#pragma omp critical
        sum += temp_sum;
    }

    assert(sum == 540);
}
