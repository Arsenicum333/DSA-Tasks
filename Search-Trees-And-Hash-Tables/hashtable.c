#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INIT_SIZE 128
#define FACTOR 0.75
#define PRIME 11

typedef struct Data {
    unsigned int id;
    char firstName[30];
    char lastName[30];
} Data;

typedef struct HashTable {
    Data* table;
    int size;
    int count;
} HashTable;

HashTable* createHashTable();
HashTable* insertHashTable(HashTable* ht, unsigned int id, const char* firstName, const char* lastName);
HashTable* resizeHashTable(HashTable* ht);
Data* searchHashTable(HashTable* ht, unsigned int id);
HashTable* deleteHashTable(HashTable* ht, unsigned int id);
void freeHashTable(HashTable* ht);

int hashFirst(int id, int size) {return id % size;}
int hashSecond(int id, int size) {return PRIME - (id % PRIME);}

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = INIT_SIZE;
    ht->count = 0;
    ht->table = (Data*)calloc(ht->size, sizeof(Data));
    return ht;
}

HashTable* insertHashTable(HashTable* ht, unsigned int id, const char* firstName, const char* lastName) {
    if ((float)ht->count / ht->size >= FACTOR)
        resizeHashTable(ht);

    int index = hashFirst(id, ht->size);
    int step = hashSecond(id, ht->size);
    int attempts = 0;

    while (ht->table[index].id != 0 && attempts < ht->size) {
        if (ht->table[index].id == id)
            return ht;

        index = (index + step) % ht->size;
        attempts++;
    }

    if (attempts >= ht->size)
        return ht;

    ht->table[index].id = id;
    strcpy(ht->table[index].firstName, firstName);
    strcpy(ht->table[index].lastName, lastName);
    ht->count++;

    return ht;
}

HashTable* resizeHashTable(HashTable* ht) {
    if (ht == NULL || ht->table == NULL)
        return ht;

    int oldSize = ht->size;
    Data* oldTable = ht->table;

    ht->size *= 2;
    ht->count = 0;
    ht->table = (Data*)calloc(ht->size, sizeof(Data));

    if (ht->table == NULL) {
        ht->table = oldTable;
        ht->size = oldSize;
        return ht;
    }

    for (int i = 0; i < oldSize; i++) {
        if (oldTable[i].id != 0) {
            insertHashTable(ht, oldTable[i].id, oldTable[i].firstName, oldTable[i].lastName);
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
            return &ht->table[index];

        index = (index + step) % ht->size;
        attempts++;
    }

    return NULL;
}

HashTable* deleteHashTable(HashTable* ht, unsigned int id) {
    int index = hashFirst(id, ht->size);
    int step = hashSecond(id, ht->size);
    int attempts = 0;

    while (ht->table[index].id != 0 && attempts < ht->size) {
        if (ht->table[index].id == id) {
            ht->table[index].id = 0;
            ht->table[index].firstName[0] = '\0';
            ht->table[index].lastName[0] = '\0';
            ht->count--;
            return ht;
        }

        index = (index + step) % ht->size;
        attempts++;
    }

    return ht;
}

void freeHashTable(HashTable* ht) {
    if (ht == NULL) return;

    free(ht->table);
    free(ht);
}
