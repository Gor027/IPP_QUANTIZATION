#ifndef QUANTIZATION_IPP_UNIONFIND_H
#define QUANTIZATION_IPP_UNIONFIND_H

#include <stdint.h>
#include <stdbool.h>

typedef struct UnionFind {
    uint64_t energy;
    struct UnionFind *parent;
} UnionFind;

UnionFind *createUnion(void);

UnionFind *findRoot(UnionFind *unionFind);

bool merge(UnionFind *a, UnionFind *b);

#endif //QUANTIZATION_IPP_UNIONFIND_H
