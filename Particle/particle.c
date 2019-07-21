#include <stdio.h>
#include <stdlib.h>
#include "particle.h"

Particle *newParticle(void) {
    Particle *particle = (Particle *) malloc(sizeof(Particle));

    if (particle == NULL)
        exit(1);

    particle->historiesRoot = newTrieNode();

    return particle;
}

bool declareHistory(Particle *particle, char *history) {
    if (history != NULL) {
        insert(particle->historiesRoot, history);
        return true;
    }

    return false;
}

bool removeHistory(Particle *particle, char *history) {
    if (history != NULL) {
        delete(particle->historiesRoot, history);
        return true;
    }

    return false;
}

bool isValid(Particle *particle, char *history) {
    if (history != NULL)
        return (search(particle->historiesRoot, history) != NULL);
    return false;
}

bool assignEnergy(Particle *particle, char *history, uint64_t energy) {
    TrieNode *trieNode = search(particle->historiesRoot, history);
    if (trieNode != NULL && energy > 0) {
        setEnergy(trieNode, energy);
        return true;
    }

    return false;
}

bool printEnergy(Particle *particle, char *history) {
    if (history != NULL) {
        TrieNode *trieNode = search(particle->historiesRoot, history);

        if (trieNode) {
            UnionFind *unionFind = trieNode->group;
            UnionFind *root = findRoot(unionFind);

            if (root->energy > 0) {
                fprintf(stdout, "%lu\n", root->energy);
                return true;
            }
        }
    }

    return false;
}

bool equal(Particle *particle, char *a, char *b) {
    TrieNode *aHistory = search(particle->historiesRoot, a);
    TrieNode *bHistory = search(particle->historiesRoot, b);

    if (aHistory != NULL && bHistory != NULL) {
        if (aHistory != bHistory) {
            UnionFind *aUnion = aHistory->group;
            UnionFind *bUnion = bHistory->group;

            if (merge(aUnion, bUnion))
                return true;

            // If aUnion and bUnion both do not have assigned energy merge() returns false
            return false;
        }
        // If histories are identical return true
        return true;
    }

    return false;
}

void freeAll(Particle *particle) {
    freeTrie(particle->historiesRoot);
    freeUnionList();
}
