#include <stdio.h>

template <int N>
struct UnionFind {
    unsigned int parent[N];

    UnionFind() {
        for (int i = 0; i < N; i++) {
            parent[i] = i;
        }
    }

    void join(int p, int q) {
        parent[root(p)] = root(q);
    }

    bool connected(int p, int q) {
        return root(p) == root(q);
    }

    private:

    int root(int node) {
        while (parent[node] != node) {
            node = parent[node];
        }
        return node;
    }
};

int main() {
    UnionFind<10> unionfind;
    unionfind.join(0, 1);
    unionfind.join(1, 2);

    printf("%d\n", unionfind.connected(0, 3));
}

