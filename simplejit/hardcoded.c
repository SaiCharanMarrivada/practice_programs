#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double r, i;
} Complex;

static inline unsigned int length_squared(Complex c) {
    return (c.r * c.r) + (c.i * c.i);
}

void interpret(Complex *registers, const char *code) {
    // hard-code the interpret loop
    Complex *a = &registers[0];
    Complex *b = &registers[1];

    double i, r;
    r = b->r * b->r - b->i * b->i;
    i = b->r * b->i + b->i * b->r;

    b->r = r + a->r;
    b->i = i + a->i;
}


int main(int argc, char *argv[]) {
    Complex registers[4];
    int i, x, y;
    char line[1600];

    // printf("%s", argv[1]);
    // exit(1);
    printf("P5\n%d %d\n%d\n", 1600, 900, 255);

    for (y = 0; y < 900; y++) {
        for (x = 0; x < 1600; x++) {
            // register 'a'
            registers[0].r = 2 * 1.6 * (x / 1600.0 - 0.5);
            registers[0].i = 2 * 0.9 * (y / 900.0 - 0.5);

            for (i = 1; i < 4; i++) {
                registers[i].r = 0;
                registers[i].i = 0;
            }

            for (i = 0; i < 256 && length_squared(registers[1]) < 4; i++)
                interpret(registers, argv[1]);
            line[x] = i;
        }
        fwrite(line, 1, sizeof(line), stdout);
    }
    return 0;
}
