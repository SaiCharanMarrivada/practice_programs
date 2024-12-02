#include <cstdio>

#define THREADS_PER_BLOCK 512

__global__ void kernel_copy(int *d_output, int *d_input, size_t size) {
    int id = blockDim.x * blockIdx.x + threadIdx.x;
    if (id < size) d_output[id] = d_input[id];
}

#define BENCHMARK(n)                                                      \
    float *bench_copy##n(int nelements, int repetitions) {                \
        int nblocks = nelements / THREADS_PER_BLOCK;                      \
        if (nblocks % THREADS_PER_BLOCK) nblocks++;                       \
        cudaEvent_t start, stop;                                          \
        cudaEventCreate(&start);                                          \
        cudaEventCreate(&stop);                                           \
        int *d_output, *d_input;                                          \
        cudaMalloc(&d_output, nelements * sizeof(int));                   \
        cudaMalloc(&d_input, nelements * sizeof(int));                    \
        float *timings = (float *)malloc(sizeof(float) * repetitions);    \
        float time_elapsed;                                               \
        for (int i = 0; i < repetitions; i++) {                           \
            cudaEventRecord(start, 0);                                    \
            CODE##n;                                                      \
            cudaEventRecord(stop, 0);                                     \
            cudaEventSynchronize(stop);                                   \
            cudaEventElapsedTime(&time_elapsed, start, stop);             \
            timings[i] = time_elapsed;                                    \
        }                                                                 \
        cudaFree(d_input);                                                \
        cudaFree(d_output);                                               \
        return timings;                                                   \
    }

#define CODE1 cudaMemcpy(d_output, d_input, nelements * sizeof(int), \
                cudaMemcpyDeviceToDevice)
#define CODE2 kernel_copy<<<nblocks, THREADS_PER_BLOCK>>>(d_output, d_input, nelements)

BENCHMARK(1);
BENCHMARK(2);

int main() {
    int nelements = (1 << 27) / sizeof(int);
    float *timings1 = bench_copy1(nelements, 10);
    float *timings2 = bench_copy2(nelements, 10);

    printf("timings1: ");
    for (int i = 0; i < 10; i++) {
        printf("%lf, ", timings1[i]);
    }
    printf("\n");

    printf("timings2: ");
    for (int i = 0; i < 10; i++) {
        printf("%lf, ", timings2[i]);
    }
    printf("\n");

    free(timings1);
    free(timings2);

}
