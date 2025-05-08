#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define INIT_SIZE 1024
#define FACTOR 0.75 // Load factor threshold for resizing

typedef struct Node Node;

typedef struct DataBDD {
    char variable;
    Node* low;
    Node* high;
    Node* node;
    bool isDeleted; // Flag for soft deletion
    struct DataBDD* next;
} DataBDD;

typedef struct HashTable {
    DataBDD** table;
    unsigned int size;
    unsigned int count;
} HashTable;

HashTable* createHashTable();
HashTable* insertHashTable(HashTable* ht, char variable, Node* low, Node* high, Node* node);
HashTable* resizeHashTable(HashTable* ht);
DataBDD* searchHashTable(HashTable* ht, char variable, Node* low, Node* high);
HashTable* deleteHashTable(HashTable* ht, char variable, Node* low, Node* high);
void freeHashTable(HashTable* ht);

unsigned int hashCombine(char variable, Node* low, Node* high) {
    uint64_t hash = (uint64_t)variable;
    hash = hash * 31 + (uintptr_t)low;
    hash = hash * 17 + (uintptr_t)high;
    return (unsigned int)(hash % 4294967291U);
}

int hash(char variable, Node* low, Node* high, unsigned int size) {
    return hashCombine(variable, low, high) % size;
}

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = INIT_SIZE;
    ht->count = 0;
    ht->table = (DataBDD**)calloc(ht->size, sizeof(DataBDD*));
    return ht;
}

HashTable* insertHashTable(HashTable* ht, char variable, Node* low, Node* high, Node* node) {
    if (ht == NULL) {
        ht = createHashTable();
        if (ht == NULL) return NULL;
    }

    if (ht->table == NULL) return NULL;

    if ((float)ht->count / ht->size >= FACTOR) {
        ht = resizeHashTable(ht);
        if (ht == NULL || ht->table == NULL) return NULL;
    }

    int index = hash(variable, low, high, ht->size);
    DataBDD* current = ht->table[index];

    // Check for existing data
    while (current) {
        if (!current->isDeleted &&
            current->variable == variable &&
            current->low == low &&
            current->high == high) {
            return ht;
        }

        current = current->next;
    }

    DataBDD* newData = (DataBDD*)malloc(sizeof(DataBDD));
    if (!newData) {
        fprintf(stderr, "Failed to allocate DataBDD at index %d\n", index);
        return NULL;
    }
    newData->variable = variable;
    newData->low = low;
    newData->high = high;
    newData->node = node;
    newData->isDeleted = false;
    newData->next = ht->table[index];
    ht->table[index] = newData;
    ht->count++;

    return ht;
}

HashTable* resizeHashTable(HashTable* ht) {
    if (ht == NULL || ht->table == NULL) return ht;

    unsigned int oldSize = ht->size;
    DataBDD** oldTable = ht->table;

    ht->size *= 2;
    ht->count = 0;
    ht->table = (DataBDD**)calloc(ht->size, sizeof(DataBDD*));

    if (ht->table == NULL) {
        ht->table = oldTable;
        ht->size = oldSize;
        fprintf(stderr, "Failed to allocate new table of size %u\n", ht->size);
        return NULL;
    }

    // Rehash all non-deleted elements into new table
    for (unsigned int i = 0; i < oldSize; i++) {
        DataBDD* current = oldTable[i];

        while (current) {
            DataBDD* next = current->next;

            if (!current->isDeleted) {
                current->next = NULL;
                int newIndex = hash(current->variable, current->low, current->high, ht->size);
                current->next = ht->table[newIndex];
                ht->table[newIndex] = current;
                ht->count++;
            } else {
                free(current);
            }

            current = next;
        }
    }

    free(oldTable);
    return ht;
}

DataBDD* searchHashTable(HashTable* ht, char variable, Node* low, Node* high) {
    if (ht == NULL || ht->table == NULL) return NULL;

    int index = hash(variable, low, high, ht->size);
    DataBDD* current = ht->table[index];

    // Search linked list for matching data
    while (current) {
        if (!current->isDeleted &&
            current->variable == variable &&
            current->low == low &&
            current->high == high) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

HashTable* deleteHashTable(HashTable* ht, char variable, Node* low, Node* high) {
    if (ht == NULL || ht->table == NULL) return ht;

    int index = hash(variable, low, high, ht->size);
    DataBDD* current = ht->table[index];
    DataBDD* previous = NULL;

    // Search and mark data as deleted
    while (current) {
        if (!current->isDeleted &&
            current->variable == variable &&
            current->low == low &&
            current->high == high) {
            current->isDeleted = true;
            ht->count--;
            return ht;
        }

        previous = current;
        current = current->next;
    }

    return ht;
}

void freeHashTable(HashTable* ht) {
    if (ht == NULL || ht->table == NULL) return;

    for (unsigned int i = 0; i < ht->size; i++) {
        DataBDD* current = ht->table[i];

        while (current) {
            DataBDD* next = current->next;
            free(current);
            current = next;
        }
    }

    free(ht->table);
    free(ht);
}
