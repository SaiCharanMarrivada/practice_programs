#include <stdio.h>
#include <stdlib.h>


typedef struct {
    double r, i;
} Complex;

static inline unsigned int length_squared(Complex c) {
    return (c.r * c.r) + (c.i * c.i); 
}

void interpret(Complex *registers, char const *code) {
    Complex *source, *target;
    double r, i;
    for (; *code; code += 3) {
        // `code[0] is the operation`
        target = &registers[code[2] - 'a'];
        source = &registers[code[1] - 'a']; 
        switch (*code) {
            case '=':
                // printf("=(%c%c)\n", code[2], code[1]);
                target->r = source->r;
                target->i = source->i;
                break;
            case '+':
                // printf("+(%c%c)\n", code[2], code[1]);
                target->r += source->r;
                target->i += source->i;
                break;
            case '*':
                // printf("*(%c%c)\n", code[2], code[1]);
                r = (source->r * target->r - source->i * target->i);
                i = (target->r * source->i + source->r * target->i);
                target->r = r;
                target->i = i;
                break;
            default:
                fprintf(stderr, "invalid instruction %s (ascii %x)\n", 
                    code, *code);
                break; 
        }
    }
    exit(1);
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
