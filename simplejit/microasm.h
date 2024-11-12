#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {
    char *code;
} Microasm;

#define xmm(n) (n)

unsigned int bytes_written = 0;

void asm_write(Microasm *a, int n, ...) {
    va_list bytes;

    va_start(bytes, n);

    for (int i = 0; i < n; i++) {
        *(a->code++) = (char)va_arg(bytes, int);
    }
    
    va_end(bytes);
}

void movsd_reg_memory(Microasm *a, char reg, char displacement) {
    asm_write(a, 5, 0xf2, 0x0f, 0x11, 0x47 | reg << 3, displacement);
    bytes_written += 5;
}

void movsd_memory_reg(Microasm *a, char displacement, char reg) {
    asm_write(a, 5, 0xf2, 0x0f, 0x10, 0x47 | reg << 3, displacement);
    bytes_written += 5;
}

void movsd_reg_reg(Microasm *a, char source, char target) {
    asm_write(a, 4, 0xf2, 0x0f, 0x11, 0xc0 | source << 3 | target);
    bytes_written += 4;
}
void mulsd_reg_reg(Microasm *a, char source, char target) {
    asm_write(a, 4, 0xf2, 0x0f, 0x59, 0xc0 | source << 3 | target);
}

void addsd_reg_reg(Microasm *a, char source, char target) { 
    asm_write(a, 4, 0xf2, 0x0f, 0x58, 0xc0 | source << 3 | target);
    bytes_written += 4;
}

void subsd_reg_reg(Microasm *a, char source, char target) { 
    asm_write(a, 4, 0xf2, 0x0f, 0x5c, 0xc0 | source << 3 | target);
    bytes_written += 4;
}

void movupd_reg_memory(Microasm *a, char reg, char displacement) {
    asm_write(a, 5, 0x66, 0x0f, 0x11, 0x47 | reg << 3, displacement);
    bytes_written += 5;
}

void movupd_memory_reg(Microasm *a, char displacement, char reg)  { 
    asm_write(a, 5, 0x66, 0x0f, 0x10, 0x47 | reg << 3, displacement);
    bytes_written += 5;
}

void addpd_memory_reg(Microasm *a, char displacement, char reg) {
    asm_write(a, 5, 0x66, 0x0f, 0x58, 0x47 | reg << 3, displacement);
    bytes_written += 5;
}
