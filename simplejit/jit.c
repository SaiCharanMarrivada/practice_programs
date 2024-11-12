#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "microasm.h"

typedef struct {
    double r, i;
} Complex;

static inline unsigned int length_squared(Complex c) {
    return (c.r * c.r) + (c.i * c.i); 
}

typedef void (*compiled)(Complex *);

void interpret(Complex *);

compiled compile(char *code) {
    char *memory = mmap(
        NULL, // address 
        4096, // size
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, // fd
        0 // offset
    );

    Microasm microasm = {.code = memory};

    char source, target;

    char const r = offsetof(Complex, r);
    // printf("offsetof(Complex, r) = %d\n", r);
    // exit(-1);
    char const i = offsetof(Complex, i);
    // printf("offset(Complex, i) = %d\n", i);
    // exit(-1);

    for (; *code; code += 3) {
        source = sizeof(Complex) * (code[1] - 'a');
        target = sizeof(Complex) * (code[2] - 'a');

        switch (*code) {
            case '=':
                movupd_memory_reg(&microasm, source, xmm(0));
                movupd_reg_memory(&microasm, xmm(0), target);
                break;

            case '+':
                movupd_memory_reg(&microasm, source, xmm(0));
                addpd_memory_reg(&microasm, target, xmm(0));
                movupd_reg_memory(&microasm, xmm(0), target);
                break;

            case '*':
                movsd_memory_reg(&microasm, source + r, xmm(0));
                movsd_memory_reg(&microasm, source + i, xmm(1));
                movsd_memory_reg(&microasm, target + r, xmm(2));
                movsd_memory_reg(&microasm, target + i, xmm(3));

                movsd_reg_reg(&microasm, xmm(0), xmm(4)); 
                mulsd_reg_reg(&microasm, xmm(2), xmm(4)); // source.r * target.r

                movsd_reg_reg(&microasm, xmm(1), xmm(5));
                mulsd_reg_reg(&microasm, xmm(3), xmm(5)); // source.i * target.i

                subsd_reg_reg(&microasm, xmm(5), xmm(4));
                movsd_reg_memory(&microasm, xmm(4), target + r);

                mulsd_reg_reg(&microasm, xmm(0), xmm(3));
                mulsd_reg_reg(&microasm, xmm(1), xmm(2));
                addsd_reg_reg(&microasm, xmm(3), xmm(2));
                movsd_reg_memory(&microasm, xmm(2), target + i);
                break;

            default:
                fprintf(stderr, "invalid instruction %s (ascii %x)\n", 
                    code, *code);
                break; 
        }
    }

    asm_write(&microasm, 1, 0xc3); // ret
    return (compiled)memory;
}


int main(int argc, char *argv[]) {
    // printf("%s\n", argv[1]);
    // exit(1);
    compiled function = compile(argv[1]);

    // asm("jmp *%0" : : "r"(function));

    Complex registers[4];
    int i, x, y;
    char line[1600];

    // printf("%s", argv[1]);
    // exit(1);
    printf("P5\n%d %d\n%d\n", 1600, 900, 255);

    for (y = 0; y < 900; y++) {
        for (x = 0; x < 1600; x++) {
            registers[0].r = 2 * 1.6 * (x / 1600.0 - 0.5);
            registers[0].i = 2 * 0.9 * (y / 900.0 - 0.5);

            for (i = 1; i < 4; i++) {
                registers[i].r = 0;
                registers[i].i = 0;
            }
            
            for (i = 0; i < 256 && length_squared(registers[1]) < 4; i++) {
                function(registers);
            }
            line[x] = i;
            // printf("%d\n", i);
        }
        // putchar(line[1]);
        // exit(1);
        fwrite(line, 1, sizeof(line), stdout);
    }
    return 0;

}
