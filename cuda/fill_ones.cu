#include <stdio.h>

__global__ void fill_ones(int *array, int size) {
    if (array[threadIdx.x] < size) array[threadIdx.x] = 1;
}

int main() {
    int size = 256;
    int *d_array;
    cudaMalloc(&d_array, size * sizeof(int));
    fill_ones<<<1, 256>>>(d_array, size);
    int h_array[256];

    cudaMemcpy(h_array, d_array, sizeof(int) * size, cudaMemcpyDeviceToHost);
    for (int i = 0; i < 256; i++) {
        printf("%d ", h_array[i]);

        if (i % 16 == 15) printf("\n");
    }
}
