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


    // mov %rdi, %rax
    *memory++ = 0x48;
    *memory++ = 0x8b;
    *memory++ = 0xc7;

    // ret
    *memory++ = 0xc3;

    return (function)(memory - 4);
}

int main() {
    function identity = compile_identity();

    identity(10);
    int i = 0;
    for (int i = 0; i < 10; i++) {
        printf("f(i) = %ld\n", identity(i));
    }

    munmap(identity, 4096);
    return 0;

}
