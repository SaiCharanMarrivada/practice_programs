#include <iostream>

template <typename Type>
void print(Type value) {
    // base case
    std::cout << value << "\n";
}

template <typename Type, typename ...Types>
void print(Type value, Types... values) {
    std::cout << value << ", ";
    print(values...);
}

int main() {
    print("string", 1.2, 1);
}
