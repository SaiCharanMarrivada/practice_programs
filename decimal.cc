#include <utility>
#include <stdio.h>

template <int i>
int case_(int n) {
    return 10 * n + i;
}

int to_decimal(const char *s) {
    bool sign;
    if (*s && *s == '-') {
        sign = true;
        s++;
    }
    // compile time branch table
    auto cases = []<int... N>(std::integer_sequence<int, N...>) {
        static constexpr int (*cases_[])(int) = {case_<N>...};
        return cases_;
    }(std::make_integer_sequence<int, 10>());

    int number = 0;
    while (*s) {
        number = cases[*s - '0'](number);
        ++s;
    }
    return (sign) ? -number : number;
}

int main() {
    const char *input = "-1234";
    printf("%d\n", to_decimal(input));
    return 0;
}
