#include "bench.h" 
#include <vector>
#include <string>

#define REPEAT2(s) s s
#define REPEAT4(s) REPEAT2(REPEAT2(s))
#define TEST_STRING REPEAT4(REPEAT4("s"))

std::vector<std::string_view> powerset(std::string input, char *current) {
    std::vector<std::string_view> sets;
    sets.reserve(1ull << input.size());

    for (size_t i = 1; i < 1ull << input.size(); i++) {
        char *s = current;
        for (int j = i; j > 0; ) {
            size_t lsb_bit = j & -j;
            *s++ = input[__builtin_ctzll(j)];
            j -= lsb_bit;
        }
        
        sets.emplace_back(current, (size_t)(s - current));
        current = s;
    }
    return sets;
}

#define SETUP  \
    char pool[16 * (1 << 15)]; \
    char *current = pool

CREATE_BENCHMARK(SETUP, powerset, TEST_STRING, current);

BENCHMARK_MAIN();
