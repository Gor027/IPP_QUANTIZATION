#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "trie.h"

// List is essential for freeing all unionFind nodes
typedef struct List {
    void *unionFind;
    struct List *next;
} List;

// Global variable to prevent carrying it in all functions
List *head = NULL;

/**
 * To free all unionFind nodes for trie nodes
 */
void freeUnionList(void) {
    while (head != NULL) {
        List *current = head;
        head = head->next;
        free(current->unionFind);
        free(current);
    }
}

/**
 * Void in brackets means no parameters are expected
 * @return
 */
TrieNode *newTrieNode(void) {
    TrieNode *trieNode = (TrieNode *) malloc(sizeof(TrieNode));

    if (trieNode == NULL)
        exit(1);

    trieNode->group = createUnion();
    List *list = (List *) malloc(sizeof(List));

    if (list == NULL)
        exit(1);

    list->unionFind = trieNode->group;
    list->next = head;
    head = list;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        trieNode->children[i] = NULL;

    return trieNode;
}

/**
 *
 * @param trieNode - given trie node
 * @param energy - specified energy to be assigned to
 */
void setEnergy(TrieNode *trieNode, uint64_t energy) {
    UnionFind *unionFind = trieNode->group;
    UnionFind *root = findRoot(unionFind);
    if (root)
        root->energy = energy;
}

/**
 * Function for inserting given key into trie
 * @param trieNode - the root of the trie
 * @param key - char sequence to be inserted into trie
 */
void insert(TrieNode *trieNode, char *key) {
    uint64_t length = strlen(key);
    struct TrieNode *current = trieNode;

    for (uint64_t i = 0; i < length; i++) {
        int index = CHAR_TO_INT(key[i]);

        if (current->children[index] == NULL)
            current->children[index] = newTrieNode();

        current = current->children[index];
    }
}

/**
 * Function to check given key is in trie or not
 * @param trieNode - the root of the trie
 * @param key - char sequence to be searched in trie
 * @return NULL if char sequence is not found in trie
 */
TrieNode *search(TrieNode *trieNode, char *key) {
    uint64_t length = strlen(key);
    struct TrieNode *current = trieNode;

    for (uint64_t i = 0; i < length; i++) {
        int index = CHAR_TO_INT(key[i]);

        if (current != NULL && current->children[index] == NULL)
            return NULL;

        if (current != NULL)
            current = current->children[index];
    }

    return (current != NULL) ? current : NULL;
}

/**
 * Function to free all nodes with trieNode as prefix node
 * @param trieNode
 */
void freeTrie(TrieNode *trieNode) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (trieNode->children[i] != NULL) {
            freeTrie(trieNode->children[i]);
        }
    }

    free(trieNode);
}

/**
 * Function for deletion of certain key
 * When key is deleted, recursively all other nodes based on that key are also deleted
 * @param trieNode
 * @param key
 */
void delete(TrieNode *trieNode, char *key) {
    uint64_t length = strlen(key);
    struct TrieNode *current = trieNode;

    for (uint64_t i = 0; i < length - 1; i++) {
        int index = CHAR_TO_INT(key[i]);

        if (current->children[index] == NULL)
            return;

        current = current->children[index];
    }

    int index = CHAR_TO_INT(key[length - 1]);

    if (current->children[index] != NULL)
        freeTrie(current->children[index]);

    current->children[index] = NULL;
}