#include <stdio.h>

typedef struct {
    int x, y;
} Point2;

typedef struct {
    Point2 point2;
    int z;
} Point3;

void print_point2(Point2 *point2) {
    printf("%d, %d", point2->x, point2->y);
}

void print_point3(Point3 *point3) {
    print_point2((Point2 *)point3);
    printf(", %d", point3->z);
}

int main() {
    Point3 p = {.point2.x = 1, .point2.y = 2, .z = 3};
    print_point3(&p);
    printf("\n");
    print_point2((Point2 *)&p);
}
