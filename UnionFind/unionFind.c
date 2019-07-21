#include <stdlib.h>
#include "unionFind.h"

/**
 * Initializes a new union with specified energy
 * @param energy
 * @return
 */
UnionFind *createUnion(void) {
    UnionFind *unionFind = (UnionFind *) malloc(sizeof(UnionFind));

    if (unionFind == NULL)
        exit(1);

    unionFind->parent = NULL;
    unionFind->energy = 0;

    return unionFind;
}

/**
 * Path compression uses recursion to compress the depth as much as possible
 * @param unionFind
 * @return
 */
UnionFind *pathCompression(UnionFind *unionFind) {
    if (unionFind->parent == NULL)
        return unionFind;
    else {
        unionFind->parent = pathCompression(unionFind->parent);
    }
    return unionFind->parent;
}

/**
 * Finds union of the given trieNode
 * @param unionFind
 * @return
 */
UnionFind *findRoot(UnionFind *unionFind) {
    if (unionFind == NULL)
        return NULL;

    if (unionFind->parent != NULL) {
        unionFind->parent = pathCompression(unionFind);
        return unionFind->parent;
    }

    return unionFind;
}

/**
 * Merges two UnionFind nodes
 * Returns true on success, otherwise false
 * a and b are UnionFind nodes of the histories to be merged
 * @param a
 * @param b
 * @return
 */
bool merge(UnionFind *a, UnionFind *b) {
    UnionFind *aUnion = findRoot(a);
    UnionFind *bUnion = findRoot(b);

    if (aUnion != bUnion) {
        if (aUnion->energy == 0 && bUnion->energy == 0) {
            return false;
        }

        if (aUnion->energy != 0 && bUnion->energy == 0)
            bUnion->parent = aUnion;
        else if (bUnion->energy != 0 && aUnion->energy == 0)
            aUnion->parent = bUnion;
        else {
            aUnion->parent = bUnion;
            bUnion->energy = (aUnion->energy / 2) + (bUnion->energy / 2) +
                             ((aUnion->energy % 2 == 1 && bUnion->energy % 2 == 1) ? 1 : 0);
        }
    }
    return true;
}
