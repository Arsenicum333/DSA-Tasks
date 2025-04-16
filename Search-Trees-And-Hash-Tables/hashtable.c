#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INIT_SIZE 1024
#define FACTOR 0.5 // Load factor threshold for resizing
#define PRIME 31 // Prime number for second hash function

typedef struct Data {
    unsigned int id;
    char firstName[20];
    char lastName[20];
    bool isDeleted; // Flag for soft deletion
} Data;

typedef struct HashTable {
    Data* table;
    unsigned int size; // Current capacity of the table
    unsigned int count; // Number of active (non-deleted) elements
} HashTable;

HashTable* createHashTable();
HashTable* insertHashTable(HashTable* ht, unsigned int id, const char* firstName, const char* lastName);
HashTable* resizeHashTable(HashTable* ht);
Data* searchHashTable(HashTable* ht, unsigned int id);
HashTable* deleteHashTable(HashTable* ht, unsigned int id);
void freeHashTable(HashTable* ht);

int hashFirst(unsigned int id, unsigned int size) {return id % size;}
int hashSecond(unsigned int id, unsigned int size) {return PRIME - (id % PRIME);}

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = INIT_SIZE;
    ht->count = 0;
    ht->table = (Data*)calloc(ht->size, sizeof(Data));
    return ht;
}

HashTable* insertHashTable(HashTable* ht, unsigned int id, const char* firstName, const char* lastName) {
    if (ht == NULL) {
        ht = createHashTable();
        if (ht == NULL) return NULL;
    }

    if ((float)ht->count / ht->size >= FACTOR) {
        ht = resizeHashTable(ht);
        if (ht == NULL) return NULL;
    }

    int index = hashFirst(id, ht->size);
    int step = hashSecond(id, ht->size);
    int attempts = 0;
    int deletedIndex = -1; // Track first deleted slot for reuse

    while (ht->table[index].id != 0 && attempts < ht->size) {
        if (ht->table[index].id == id && !ht->table[index].isDeleted)
            return ht; // If duplicate, skip insertion

        if (ht->table[index].isDeleted && deletedIndex == -1)
            deletedIndex = index; // Mark first deleted slot

        index = (index + step) % ht->size; // Move to next slot using double hashing
        attempts++;
    }

    // Use deleted slot if available, otherwise current index
    int target = (deletedIndex != -1) ? deletedIndex : index;

    ht->table[target].id = id;
    strcpy(ht->table[target].firstName, firstName);
    strcpy(ht->table[target].lastName, lastName);
    ht->table[target].isDeleted = false;
    ht->count++;

    return ht;
}

HashTable* resizeHashTable(HashTable* ht) {
    if (ht == NULL || ht->table == NULL) return ht;

    unsigned int oldSize = ht->size;
    Data* oldTable = ht->table;

    ht->size *= 2;
    ht->count = 0;
    ht->table = (Data*)calloc(ht->size, sizeof(Data));

    if (ht->table == NULL) {
        ht->table = oldTable;
        ht->size = oldSize;
        return ht;
    }

    // Rehash all non-deleted elements into new table
    for (unsigned int i = 0; i < oldSize; i++) {
        if (oldTable[i].id != 0 && !oldTable[i].isDeleted) {
            unsigned int index = hashFirst(oldTable[i].id, ht->size);
            unsigned int step = hashSecond(oldTable[i].id, ht->size);

            while (ht->table[index].id != 0)
                index = (index + step) % ht->size; // Move to next slot using double hashing

            ht->table[index] = oldTable[i];
            ht->count++;
        }
    }

    free(oldTable);
    return ht;
}

Data* searchHashTable(HashTable* ht, unsigned int id) {
    if (ht == NULL) return NULL;

    int index = hashFirst(id, ht->size);
    int step = hashSecond(id, ht->size);
    int attempts = 0;

    while (ht->table[index].id != 0 && attempts < ht->size) {
        if (ht->table[index].id == id)
            return &ht->table[index]; // Return pointer to found element

        index = (index + step) % ht->size; // Move to next slot using double hashing
        attempts++;
    }

    return NULL;
}

HashTable* deleteHashTable(HashTable* ht, unsigned int id) {
    if (ht == NULL) return ht;

    int index = hashFirst(id, ht->size);
    int step = hashSecond(id, ht->size);
    int attempts = 0;

    while (ht->table[index].id != 0 && attempts < ht->size) {
        if (ht->table[index].id == id && !ht->table[index].isDeleted) {
            ht->table[index].isDeleted = true; // Mark element as deleted for soft deletion
            ht->count--;
            return ht;
        }

        index = (index + step) % ht->size; // Move to next slot using double hashing
        attempts++;
    }

    return ht;
}

void freeHashTable(HashTable* ht) {
    if (ht == NULL) return;

    free(ht->table);
    free(ht);
}
