#ifndef QUANTIZATION_IPP_PARTICLE_H
#define QUANTIZATION_IPP_PARTICLE_H

#include <stdbool.h>
#include <stdint.h>
#include "../Trie/trie.h"

typedef struct Particle {
    TrieNode *historiesRoot;
} Particle;

Particle *newParticle(void);

bool declareHistory(Particle *particle, char *history);

bool removeHistory(Particle *particle, char *history);

bool isValid(Particle *particle, char *history);

bool assignEnergy(Particle *particle, char *history, uint64_t energy);

bool printEnergy(Particle *particle, char *history);

bool equal(Particle *particle, char *a, char *b);

void freeAll(Particle *particle);

#endif //QUANTIZATION_IPP_PARTICLE_H
