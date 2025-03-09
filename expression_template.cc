#include <initializer_list>
#include <iostream>

template <typename E>
struct Expression {
    inline const E &self(void) const {
        return *static_cast<const E *>(this);
    }
};

template <typename T>
struct Vector : Expression<Vector<T>> {
    T x[3];

    Vector(std::initializer_list<T> list) {
        std::copy(list.begin(), list.end(), x);
    }

    // Force the evaluation by calling the constructor
    template <typename E>
    inline Vector(const Expression<E> &e) {
        auto const &expr = e.self();
        for (int i = 0; i < 3; i++) {
            x[i] = expr[i];
        }
    }

    T operator[](int i) const {
        return x[i];
    }

    T &operator[](int i) {
        return x[i];
    }

};

template <template<typename> class F, typename E1, typename E2>
struct Map : Expression<Map<F, E1, E2>> {
    const E1 &left;
    const E2 &right;
    Map<F, E1, E2>(const E1 &e1, const E2 &e2) : left(e1), right(e2) {}
    float operator[](int i) const {
        return F{}(left[i], right[i]);
    }
};

template <typename E1, typename E2>
auto operator+(const Expression<E1> &e1, const Expression<E2> &e2) {
    return Map<std::plus, E1, E2>(e1.self(), e2.self());
}

template <typename E1, typename E2>
auto operator*(const Expression<E1> &e1, const Expression<E2> &e2) {
    return Map<std::multiplies, E1, E2>(e1.self(), e2.self());
}

template <typename E1, typename E2>
auto operator/(const Expression<E1> &e1, const Expression<E2> &e2) {
    return Map<std::divides, E1, E2>(e1.self(), e2.self());
}

template <typename E1, typename E2>
auto operator-(const Expression<E1> &e1, const Expression<E2> &e2) {
    return Map<std::minus, E1, E2>(e1.self(), e2.self());
}

Vector<float> expression1(Vector<float> v1, Vector<float> v2, Vector<float> v3) {
    Vector<float> v{};
    for (int i = 0; i < 3; i++) {
        v[i] = v1[i] * v2[i] / v3[i];
    }
    return v;
}

Vector<float> expression2(Vector<float> v1, Vector<float> v2, Vector<float> v3) {
    return v1 * v2 / v3;
}

int main() {
    Vector<float> v = {1, 2, 3};
    Vector<float> u = {2, 3, 4};
    Vector<float> w = u + v;
    w[0] = 2;

    for (int i = 0; i < 3; i++) {
        std::cout << w[i] << ", ";
    }
}
