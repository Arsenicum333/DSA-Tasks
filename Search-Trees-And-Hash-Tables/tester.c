#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define NUM_OPERATIONS 100000000
#define MAX_ID 1000000000

typedef struct Data {
    unsigned int id;
    char firstName[30];
    char lastName[30];
} HashData;

typedef struct HashTable {
    HashData** table;
    int size;
    int count;
} HashTable;

HashTable* createHashTable(void);
HashTable* insertHashTable(HashTable* ht, int id, const char* firstName, const char* lastName);
HashData* searchHashTable(HashTable* ht, int id);
HashTable* deleteHashTable(HashTable* ht, int id);
void freeHashTable(HashTable* ht);

typedef struct WBTree {
    unsigned int id;
    char firstName[30];
    char lastName[30];
    int size;
    struct WBTree* leftWBTree;
    struct WBTree* rightWBTree;
} WBTree;

WBTree* createWBTree(unsigned int id, const char *firstName, const char *lastName);
WBTree* insertWBTree(WBTree *root, unsigned int id, const char *firstName, const char *lastName);
WBTree* searchWBTree(WBTree *root, unsigned int id);
WBTree* deleteWBTree(WBTree* root, unsigned int id);
void freeWBTree(WBTree* root);

unsigned int generateRandomID() {return 1 + (unsigned int)(((double)rand() / RAND_MAX) * (MAX_ID - 1));}

SIZE_T getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        return pmc.WorkingSetSize;

    return 0;
}

void testHashTable(int* insertedIDs, int* storedIDs, const char* firstName, const char* lastName) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    HashTable* ht = createHashTable();
    SIZE_T memoryBefore = getMemoryUsage();

    int inserts = 0, searches = 0, deletes = 0;

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        int operation = rand() % 3; // 0 - Insert, 1 - Search, 2 - Delete
        int randomID = generateRandomID();

        switch (operation) {
            case 0: // Insert
                insertHashTable(ht, randomID, firstName, lastName);
                inserts++;
                if (*storedIDs < NUM_OPERATIONS) {
                    insertedIDs[*storedIDs] = randomID;
                    (*storedIDs)++;
                }
                break;
            case 1: // Search
                if (*storedIDs > 0) {
                    int searchID = insertedIDs[rand() % *storedIDs];
                    searchHashTable(ht, searchID);
                    searches++;
                }
                break;
            case 2: // Delete
                if (*storedIDs > 0) {
                    int deleteID = insertedIDs[rand() % *storedIDs];
                    deleteHashTable(ht, deleteID);
                    deletes++;
                }
                break;
        }
    }

    QueryPerformanceCounter(&end);
    SIZE_T memoryAfter = getMemoryUsage();
    double timeSpent = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    double memoryUsageKB = (double)(memoryAfter - memoryBefore) / 1024.0;
    double avgTimePerOp = timeSpent / NUM_OPERATIONS * 1000000.0;

    printf("Execution time: %.6f seconds\n", timeSpent);
    printf("Peak memory usage: %.2f KB\n", memoryUsageKB);
    printf("Remaining elements: %d\n", ht->count);
    printf("Operations:\nInserts = %d\nSearches = %d\nDeletes = %d\n", inserts, searches, deletes);
    printf("Average time per operation: %.2f microseconds\n", avgTimePerOp);

    freeHashTable(ht);
}

void testWBTree(int* insertedIDs, int* storedIDs, const char* firstName, const char* lastName) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    WBTree* root = NULL;
    SIZE_T memoryBefore = getMemoryUsage();

    int inserts = 0, searches = 0, deletes = 0;

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        int operation = rand() % 3; // 0 - Insert, 1 - Search, 2 - Delete
        int randomID = generateRandomID();

        switch (operation) {
            case 0: // Insert
                root = insertWBTree(root, randomID, firstName, lastName);
                inserts++;
                if (*storedIDs < NUM_OPERATIONS) {
                    insertedIDs[*storedIDs] = randomID;
                    (*storedIDs)++;
                }
                break;
            case 1: // Search
                if (*storedIDs > 0) {
                    int searchID = insertedIDs[rand() % *storedIDs];
                    searchWBTree(root, searchID);
                    searches++;
                }
                break;
            case 2: // Delete
                if (*storedIDs > 0) {
                    int deleteID = insertedIDs[rand() % *storedIDs];
                    root = deleteWBTree(root, deleteID);
                    deletes++;
                }
                break;
        }
    }

    QueryPerformanceCounter(&end);
    SIZE_T memoryAfter = getMemoryUsage();
    double timeSpent = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    double memoryUsageKB = (double)(memoryAfter - memoryBefore) / 1024.0;
    double avgTimePerOp = timeSpent / NUM_OPERATIONS * 1000000.0;
    int treeSize = (root != NULL) ? root->size : 0;

    printf("Execution time: %.6f seconds\n", timeSpent);
    printf("Peak memory usage: %.2f KB\n", memoryUsageKB);
    printf("Remaining elements: %d\n", treeSize);
    printf("Operations:\nInserts = %d\nSearches = %d\nDeletes = %d\n", inserts, searches, deletes);
    printf("Average time per operation: %.2f microseconds\n", avgTimePerOp);

    freeWBTree(root);
}

int main() {
    srand((unsigned int)time(NULL));
    const char* firstName = "John";
    const char* lastName = "Doe";

    int* insertedIDs = (int*)malloc(NUM_OPERATIONS * sizeof(int));
    int storedIDs = 0;

    printf("Testing HashTable...\n");
    testHashTable(insertedIDs, &storedIDs, firstName, lastName);

    storedIDs = 0;
    printf("\nTesting WBTree...\n");
    testWBTree(insertedIDs, &storedIDs, firstName, lastName);

    free(insertedIDs);

    return 0;
}