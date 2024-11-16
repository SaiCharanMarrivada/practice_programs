#include <stdio.h>

typedef struct {
    union as {
        float float32;
        int int32;
    } as;
} Bits;

float negate(float x) {
    Bits y = {.as.float32 = x};
    Bits negative0 = {.as.float32 = -0.0f};
    Bits negated = {.as.int32 = y.as.int32 ^ negative0.as.int32};
    return negated.as.float32;
}

int main() {
    printf("%f\n", negate(-1));
}
