unsigned int abs(int a) {
    return (a ^ (a >> 31)) - (a >> 31);
}

unsigned int cube_root(unsigned int n) {
    unsigned int bitwidth = 31 - __builtin_clz(n);
    
    unsigned int b = (1 << (bitwidth / 3));
    unsigned int a;
    do {
        a = (2 * b + n / (b * b)) / 3;
        b = a;
    } while (abs(a - b) > 1);

    return a;
}
