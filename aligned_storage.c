#include <stdalign.h>
#include <xmmintrin.h>

#ifdef __cplusplus
#define _Alignas alignas
#endif

typedef struct {
    _Alignas(16) float x[4];
} Vector4;

Vector4 add(Vector4 v1, Vector4 v2) {
    Vector4 v;
    for (int i = 0; i < 4; i++) {
        v.x[i] = v1.x[i] + v2.x[i];
    }
    return v;
}
