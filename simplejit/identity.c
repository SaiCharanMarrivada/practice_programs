#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef long (*function)(long);

function compile_identity(void) {
    //`PROT_EXEC` is not set for memory allocated with `malloc`
    char *memory = mmap(
        NULL, // address 
        4096, // size
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, // fd
        0 // offset
    );

    if (memory == MAP_FAILED) {
        perror("failed to allocate memory");
        exit(1);
    }

    int i = 0;

    // mov %rdi, %rax
    memory[i++] = 0x48;
    memory[i++] = 0x8b;
    memory[i++] = 0xc7;

    // ret
    memory[i++] = 0xc3;

    return (function)memory;
}

int main() {
    function identity = compile_identity();

    int i = 0;
    for (int i = 0; i < 10; i++) {
        printf("f(i) = %ld\n", identity(i));
    }

    munmap(identity, 4096);
    return 0;

}
