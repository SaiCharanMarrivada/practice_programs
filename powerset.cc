#include "bench.h"
#include <vector>
#include <string>

#define REPEAT2(s) s s
#define REPEAT4(s) REPEAT2(REPEAT2(s))
#define TEST_STRING REPEAT4(REPEAT4("s"))


std::vector<std::string> powerset(std::string input) {
    std::vector<std::string> sets;
    sets.reserve(1ull << input.size());

    for (size_t i = 1; i < 1ull << input.size(); i++) {
        std::string s;
        s.reserve(__builtin_popcountll(i));
        int j = i;
        while (j > 0) {
            size_t lsb_bit = j & -j;
            s.push_back(input[__builtin_ctz(lsb_bit)]);
            j -= lsb_bit;
        }
        sets.push_back(std::move(s));
    }
    return sets;
}

#define SETUP
CREATE_BENCHMARK(SETUP, powerset, TEST_STRING);

BENCHMARK_MAIN();
