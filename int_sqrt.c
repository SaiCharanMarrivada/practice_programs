#include <stdio.h>
#include <omp.h>

unsigned int abs(int n) {
    return (n ^ (n >> 31)) - (n >> 31);
}

unsigned int int_sqrt(unsigned int n) {
    unsigned int bitwidth = 31 - __builtin_clz(n); 
    unsigned int a = 1 << (bitwidth >> 1); 
    unsigned int b = n;

    while (abs(a - b) > 1) {
        b = n / a;
        a = (a + b) / 2;
    }

    return a - (a * a > n);

}

int main() {
    double t = omp_get_wtime();
    int y = int_sqrt(1100000);
    printf("time elapsed: %lf, y = %u\n", omp_get_wtime() - t, y);
}
