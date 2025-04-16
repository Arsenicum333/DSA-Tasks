#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "w-b-tree.c"
#include "treap-tree.c"
#include "hashtable.c"

void testWBTree(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    WBTree* root = NULL;

    double totalInsertTime = 0.0;
    double totalSearchTime = 0.0;
    double totalDeleteTime = 0.0;
    int inserts = 0, searches = 0, deletes = 0;
    char line[100];
    unsigned int id;
    char firstName[30];
    char lastName[30];

    while (fgets(line, sizeof(line), file)) {
        clock_t start, end;

        if (line[0] == 'i') {  // Insert
            sscanf(line, "i %u %s %s", &id, firstName, lastName);
            start = clock();
            root = insertWBTree(root, id, firstName, lastName);
            end = clock();
            totalInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
            inserts++;
        }
        else if (line[0] == 's') {  // Search
            sscanf(line, "s %u", &id);
            start = clock();
            searchWBTree(root, id);
            end = clock();
            totalSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
            searches++;
        }
        else if (line[0] == 'd') {  // Delete
            sscanf(line, "d %u", &id);
            start = clock();
            root = deleteWBTree(root, id);
            end = clock();
            totalDeleteTime += (double)(end - start) / CLOCKS_PER_SEC;
            deletes++;
        }
    }

    double totalTime = totalInsertTime + totalSearchTime + totalDeleteTime;
    double avgTimePerOp = totalTime / (inserts + searches + deletes) * 1000000.0; // Average time per operation in microseconds

    printf("\n+-----------------------------------+\n");
    printf("|         WBTree Test Results       |\n");
    printf("+-----------------------------------+\n");
    printf("| Total Execution Time | %.6f s |\n", totalTime);
    printf("| Insert Time          | %.6f s |\n", totalInsertTime);
    printf("| Search Time          | %.6f s |\n", totalSearchTime);
    printf("| Delete Time          | %.6f s |\n", totalDeleteTime);
    printf("+-----------------------------------+\n");
    printf("| Inserts              | %10d |\n", inserts);
    printf("| Searches             | %10d |\n", searches);
    printf("| Deletes              | %10d |\n", deletes);
    printf("+-----------------------------------+\n");
    printf("| Avg Time per Op      |    %.2f us |\n", avgTimePerOp);
    printf("+-----------------------------------+\n");

    fclose(file);
    freeWBTree(root);
}

void testTreapTree(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    Treap* root = NULL;

    double totalInsertTime = 0.0;
    double totalSearchTime = 0.0;
    double totalDeleteTime = 0.0;
    int inserts = 0, searches = 0, deletes = 0;
    char line[100];
    unsigned int id;
    char firstName[30];
    char lastName[30];

    while (fgets(line, sizeof(line), file)) {
        clock_t start, end;

        if (line[0] == 'i') {  // Insert
            sscanf(line, "i %u %s %s", &id, firstName, lastName);
            start = clock();
            root = insertTreap(root, id, firstName, lastName);
            end = clock();
            totalInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
            inserts++;
        }
        else if (line[0] == 's') {  // Search
            sscanf(line, "s %u", &id);
            start = clock();
            searchTreap(root, id);
            end = clock();
            totalSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
            searches++;
        }
        else if (line[0] == 'd') {  // Delete
            sscanf(line, "d %u", &id);
            start = clock();
            root = deleteTreap(root, id);
            end = clock();
            totalDeleteTime += (double)(end - start) / CLOCKS_PER_SEC;
            deletes++;
        }
    }

    double totalTime = totalInsertTime + totalSearchTime + totalDeleteTime;
    double avgTimePerOp = totalTime / (inserts + searches + deletes) * 1000000.0; // Average time per operation in microseconds

    printf("\n+-----------------------------------+\n");
    printf("|       TreapTree Test Results      |\n");
    printf("+-----------------------------------+\n");
    printf("| Total Execution Time | %.6f s |\n", totalTime);
    printf("| Insert Time          | %.6f s |\n", totalInsertTime);
    printf("| Search Time          | %.6f s |\n", totalSearchTime);
    printf("| Delete Time          | %.6f s |\n", totalDeleteTime);
    printf("+-----------------------------------+\n");
    printf("| Inserts              | %10d |\n", inserts);
    printf("| Searches             | %10d |\n", searches);
    printf("| Deletes              | %10d |\n", deletes);
    printf("+-----------------------------------+\n");
    printf("| Avg Time per Op      |    %.2f us |\n", avgTimePerOp);
    printf("+-----------------------------------+\n");

    fclose(file);
    freeTreap(root);
}

void testHashTable(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    HashTable* ht = createHashTable();

    double totalInsertTime = 0.0;
    double totalSearchTime = 0.0;
    double totalDeleteTime = 0.0;
    int inserts = 0, searches = 0, deletes = 0;
    char line[100];
    unsigned int id;
    char firstName[30];
    char lastName[30];

    while (fgets(line, sizeof(line), file)) {
        clock_t start, end;

        if (line[0] == 'i') {  // Insert
            sscanf(line, "i %u %s %s", &id, firstName, lastName);
            start = clock();
            insertHashTable(ht, id, firstName, lastName);
            end = clock();
            totalInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
            inserts++;
        }
        else if (line[0] == 's') {  // Search
            sscanf(line, "s %u", &id);
            start = clock();
            searchHashTable(ht, id);
            end = clock();
            totalSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
            searches++;
        }
        else if (line[0] == 'd') {  // Delete
            sscanf(line, "d %u", &id);
            start = clock();
            deleteHashTable(ht, id);
            end = clock();
            totalDeleteTime += (double)(end - start) / CLOCKS_PER_SEC;
            deletes++;
        }
    }

    double totalTime = totalInsertTime + totalSearchTime + totalDeleteTime;
    double avgTimePerOp = totalTime / (inserts + searches + deletes) * 1000000.0; // Average time per operation in microseconds

    printf("\n+-----------------------------------+\n");
    printf("|       HashTable Test Results      |\n");
    printf("+-----------------------------------+\n");
    printf("| Total Execution Time | %.6f s |\n", totalTime);
    printf("| Insert Time          | %.6f s |\n", totalInsertTime);
    printf("| Search Time          | %.6f s |\n", totalSearchTime);
    printf("| Delete Time          | %.6f s |\n", totalDeleteTime);
    printf("+-----------------------------------+\n");
    printf("| Inserts              | %10d |\n", inserts);
    printf("| Searches             | %10d |\n", searches);
    printf("| Deletes              | %10d |\n", deletes);
    printf("+-----------------------------------+\n");
    printf("| Avg Time per Op      |    %.2f us |\n", avgTimePerOp);
    printf("+-----------------------------------+\n");

    fclose(file);
    freeHashTable(ht);
}

int main() {
    printf("\nTesting WBTree 1 000 000\n");
    testWBTree("Operations/operations1.txt");

    printf("\nTesting WBTree 2 000 000\n");
    testWBTree("Operations/operations2.txt");

    // printf("\nTesting WBTree 5 000 000\n");
    // testWBTree("Operations/operations5.txt");

    // printf("\nTesting WBTree 10 000 000\n");
    // testWBTree("Operations/operations10.txt");

    // printf("\nTesting WBTree 20 000 000\n");
    // testWBTree("Operations/operations20.txt");

    // printf("\nTesting WBTree 30 000 000\n");
    // testWBTree("Operations/operations30.txt");

    printf("\nTesting TreapTree 1 000 000\n");
    testTreapTree("Operations/operations1.txt");

    printf("\nTesting TreapTree 2 000 000\n");
    testTreapTree("Operations/operations2.txt");

    // printf("\nTesting TreapTree 5 000 000\n");
    // testTreapTree("Operations/operations5.txt");

    // printf("\nTesting TreapTree 10 000 000\n");
    // testTreapTree("Operations/operations10.txt");

    // printf("\nTesting TreapTree 20 000 000\n");
    // testTreapTree("Operations/operations20.txt");

    // printf("\nTesting TreapTree 30 000 000\n");
    // testTreapTree("Operations/operations30.txt");

    printf("\nTesting HashTable 1 000 000\n");
    testHashTable("Operations/operations1.txt");

    printf("\nTesting HashTable 2 000 000\n");
    testHashTable("Operations/operations2.txt");

    // printf("\nTesting HashTable 5 000 000\n");
    // testHashTable("Operations/operations5.txt");

    // printf("\nTesting HashTable 10 000 000\n");
    // testHashTable("Operations/operations10.txt");

    // printf("\nTesting HashTable 20 000 000\n");
    // testHashTable("Operations/operations20.txt");

    // printf("\nTesting HashTable 30 000 000\n");
    // testHashTable("Operations/operations30.txt");

    return 0;
}
