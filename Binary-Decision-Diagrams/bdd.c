#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "hashtable.c"

typedef struct Node {
    char variable;
    Node* low;
    Node* high;
    bool isLeaf;
    char value;
    int refCount;
} Node;

typedef struct BDD {
    int numVariables;
    int numNodes;
    Node* root;
    char* varOrder;
    HashTable* nodeCache;
} BDD;

Node* createNode(BDD* bdd, char variable, Node* low, Node* high);
Node* createLeaf(BDD* bdd, char value);
Node* buildBDDFromExpression(BDD* bdd, const char* expression, int* index, int level, char* order);
Node* buildVariableBDD(BDD* bdd, char variable);
Node* combineBDDs(BDD* bdd, char operation, Node* leftBDD, Node* rightBDD, int level, char* order);
BDD* createBDD(const char* expression, const char* varOrder);
BDD* createBDDWithBestOrder(const char* expression);
void shuffleOrder(char* order, int n);
void freeNode(Node* node);
void freeBDD(BDD* bdd);

Node* createNode(BDD* bdd, char variable, Node* low, Node* high) {
    if (low == high) return low; // Skip creation if low and high branches are identical

    DataBDD* data = searchHashTable(bdd->nodeCache, variable, low, high); // Check cache for existing node

    if (data) {
        data->node->refCount++;
        return data->node;
    }

    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for node (variable=%c)\n", variable);
        return NULL;
    }
    node->variable = variable;
    node->low = low;
    node->high = high;
    node->isLeaf = false;
    node->refCount = 1;
    bdd->nodeCache = insertHashTable(bdd->nodeCache, variable, low, high, node);
    if (!bdd->nodeCache) {
        free(node);
        fprintf(stderr, "Failed to insert node into hash table\n");
        return NULL;
    }
    bdd->numNodes++;
    return node;
}

Node* createLeaf(BDD* bdd, char value) {
    Node* leaf = (Node*)malloc(sizeof(Node));
    if (!leaf) {
        fprintf(stderr, "Memory allocation failed for leaf (value=%c)\n", value);
        return NULL;
    }
    leaf->isLeaf = true;
    leaf->value = value;
    leaf->refCount = 1;
    bdd->numNodes++;
    return leaf;
}

Node* buildBDDFromExpression(BDD* bdd, const char* expression, int* index, int level, char* order) {
    Node* result = NULL;

    // Parse term (e.g., !AB)
    while (expression[*index] == '!' || (expression[*index] >= 'A' && expression[*index] <= 'Z')) {
        Node* node;

        if (expression[*index] == '!') {
            (*index)++;

            if (expression[*index] < 'A' || expression[*index] > 'Z') {
                fprintf(stderr, "Invalid expression at index %d: %c\n", *index, expression[*index]);
                return result;
            }

            char variable = expression[*index];
            (*index)++;
            node = buildVariableBDD(bdd, variable);
            node = createNode(bdd, node->variable, node->high, node->low);
        } else {
            node = buildVariableBDD(bdd, expression[*index]);
            (*index)++;
        }

        if (result)
            result = combineBDDs(bdd, 'A', result, node, level, order); // Combine with AND operation
        else
            result = node;
    }

    // Parse OR terms (e.g., +D!C)
    while (expression[*index] == '+') {
        (*index)++;
        Node* next = buildBDDFromExpression(bdd, expression, index, level, order);
        if (!next) break;
        result = combineBDDs(bdd, 'O', result, next, level, order); // Combine with OR operation
    }

    return result;
}

Node* buildVariableBDD(BDD* bdd, char variable) {
    Node* leaf0 = createLeaf(bdd, '0');
    Node* leaf1 = createLeaf(bdd, '1');
    return createNode(bdd, variable, leaf0, leaf1);
}

Node* combineBDDs(BDD* bdd, char operation, Node* leftBDD, Node* rightBDD, int level, char* order) {
    if (leftBDD->isLeaf && rightBDD->isLeaf) {
        char result;

        if (operation == 'A') {
            if (leftBDD->value == '1' && rightBDD->value == '1')
                result = '1';
            else
                result = '0';
        } else {
            if (leftBDD->value == '1' || rightBDD->value == '1')
                result = '1';
            else
                result = '0';
        }

        return createLeaf(bdd, result);
    }

    char variable = order[level];

    // Use node if leaf or variable differs, else take low branch
    Node* leftLow;
    if (leftBDD->isLeaf || leftBDD->variable != variable)
        leftLow = leftBDD;
    else
        leftLow = leftBDD->low;

    // Use node if leaf or variable differs, else take high branch
    Node* leftHigh;
    if (leftBDD->isLeaf || leftBDD->variable != variable)
        leftHigh = leftBDD;
    else
        leftHigh = leftBDD->high;

    // Use node if leaf or variable differs, else take low branch
    Node* rightLow;
    if (rightBDD->isLeaf || rightBDD->variable != variable)
        rightLow = rightBDD;
    else
        rightLow = rightBDD->low;

    // Use node if leaf or variable differs, else take high branch
    Node* rightHigh;
    if (rightBDD->isLeaf || rightBDD->variable != variable)
        rightHigh = rightBDD;
    else
        rightHigh = rightBDD->high;

    Node* low = combineBDDs(bdd, operation, leftLow, rightLow, level + 1, order);
    Node* high = combineBDDs(bdd, operation, leftHigh, rightHigh, level + 1, order);
    return createNode(bdd, variable, low, high);
}

BDD* createBDD(const char* expression, const char* varOrder) {
    BDD* bdd = (BDD*)malloc(sizeof(BDD));
    bdd->numVariables = strlen(varOrder);
    bdd->numNodes = 0;
    bdd->varOrder = strdup(varOrder);
    bdd->nodeCache = createHashTable();
    int index = 0;
    Node* root = buildBDDFromExpression(bdd, expression, &index, 0, bdd->varOrder);
    if (root)
        bdd->root = root;
    else
        bdd->root = createLeaf(bdd, '0');
    return bdd;
}

BDD* createBDDWithBestOrder(const char* expression) {
    bool variables[26] = {false}; // Track unique variables
    int numVariables = 0;

    // Identify variables in expression
    for (int i = 0; expression[i]; i++) {
        if (expression[i] >= 'A' && expression[i] <= 'Z' && !variables[expression[i] - 'A']) {
            variables[expression[i] - 'A'] = true;
            numVariables++;
        }
    }

    char order[26];
    bool used[26] = {false};

    // Build initial order from unique variables
    for (int i = 0; i < numVariables; i++) {
        int varIndex = 0;

        while (used[varIndex] || !variables[varIndex])
            varIndex++;

        order[i] = 'A' + varIndex;
        used[varIndex] = true;
    }

    order[numVariables] = '\0';
    BDD* bestBDD = NULL;
    int minNodes = 999999;

    for (int i = 0; i < numVariables; i++) {
        shuffleOrder(order, numVariables);
        BDD* bdd = createBDD(expression, order);

        if (bdd->numNodes < minNodes) {
            minNodes = bdd->numNodes;
            if (bestBDD) freeBDD(bestBDD);
            bestBDD = bdd;
        } else {
            freeBDD(bdd);
        }
    }

    return bestBDD;
}

// Fisher–Yates shuffle
void shuffleOrder(char* order, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }
}

char useBDD(BDD* bdd, const char* inputs) {
    if (!bdd || !inputs || !bdd->root || !bdd->varOrder || strlen(inputs) != bdd->numVariables) return -1;

    bool used[26] = {false}; // Track used variables for validation

    // Verify variable order uniqueness
    for (int i = 0; i < bdd->numVariables; i++) {
        int index = bdd->varOrder[i] - 'A';
        if (index < 0 || index >= 26 || used[index]) return -1;
        used[index] = true;
    }

    int varIndexMap[26];

    // Map variables to input indices
    for (int i = 0; i < bdd->numVariables; i++)
        varIndexMap[bdd->varOrder[i] - 'A'] = i;

    Node* current = bdd->root;

    // Follow path based on input values
    while (!current->isLeaf) {
        int varIndex = varIndexMap[current->variable - 'A'];
        if (inputs[varIndex] != '0' && inputs[varIndex] != '1') return -1;
        current = (inputs[varIndex] == '0') ? current->low : current->high;
    }

    return current->value;
}

void freeNode(Node* node) {
    if (!node || --node->refCount > 0) return;
    if (!node->isLeaf) {
        freeNode(node->low);
        freeNode(node->high);
    }
    free(node);
}

void freeBDD(BDD* bdd) {
    if (!bdd) return;
    freeNode(bdd->root);
    freeHashTable(bdd->nodeCache);
    free(bdd->varOrder);
    free(bdd);
}
