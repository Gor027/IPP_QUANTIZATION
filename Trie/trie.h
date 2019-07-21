#ifndef QUANTIZATION_IPP_TRIE_H
#define QUANTIZATION_IPP_TRIE_H

#include <stdbool.h>
#include <stdint.h>
#include "../UnionFind/unionFind.h"

#define ALPHABET_SIZE 4
#define CHAR_TO_INT(c) ((int)(c) - (int)('0'))

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    UnionFind *group;
} TrieNode;

void freeUnionList(void);

TrieNode *newTrieNode(void);

void setEnergy(TrieNode *trieNode, uint64_t energy);

void insert(TrieNode *trieNode, char *key);

TrieNode *search(TrieNode *trieNode, char *key);

void freeTrie(TrieNode *trieNode);

void delete(TrieNode *trieNode, char *key);

#endif //QUANTIZATION_IPP_TRIE_H