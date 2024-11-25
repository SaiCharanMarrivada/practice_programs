import networkx as nx
import random


class UnionFind:
    def __init__(self, n):
        self.parent = [node for node in range(n)]

    def root(self, node):
        while self.parent[node] != node:
            node = self.parent[node]
        return node

    def connected(self, p, q):
        return self.root(p) == self.root(q)

    def union(self, p, q):
        self.parent[self.root(p)] = self.root(q)


class WeightedUnionFind(UnionFind):
    def __init__(self, n):
        super().__init__(n)
        self.weight = [1 for node in self.parent]

    def union(self, p, q):
        root_p = self.root(p)
        root_q = self.root(q)
        if root_p != root_q:
            if self.weight[root_p] < self.weight[root_q]:
                self.parent[root_p] = root_q
                self.weight[root_q] += self.weight[root_p]
            else:
                self.parent[root_q] = root_p
                self.weight[root_p] += self.weight[root_q]


unionfind = WeightedUnionFind(10)
for node in random.sample(range(10), 5):
    unionfind.union(0, node)
    assert unionfind.connected(node, 0)

print(unionfind.parent)
