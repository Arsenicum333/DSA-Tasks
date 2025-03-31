#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ALPHA 0.25

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
void updateSize(WBTree *node);
WBTree* balanceWBTree(WBTree *node);
bool isUnbalanced(WBTree *node);
WBTree* rotateLeft(WBTree *root);
WBTree* rotateRight(WBTree *root);
WBTree* searchWBTree(WBTree *root, unsigned int id);
WBTree* deleteWBTree(WBTree* root, unsigned int id);
void freeWBTree(WBTree* root);

WBTree* createWBTree(unsigned int id, const char *firstName, const char *lastName) {
    WBTree *node = (WBTree*)calloc(1, sizeof(WBTree));
    node->id = id;
    strcpy(node->firstName, firstName);
    strcpy(node->lastName, lastName);
    node->size = 1;
    node->leftWBTree = NULL;
    node->rightWBTree = NULL;
    return node;
}

WBTree* insertWBTree(WBTree *root, unsigned int id, const char *firstName, const char *lastName) {
    if (root == NULL)
        return createWBTree(id, firstName, lastName);

    if (root->leftWBTree == NULL && id < root->id)
        root->leftWBTree = createWBTree(id, firstName, lastName);
    else if (root->rightWBTree == NULL && id > root->id)
        root->rightWBTree = createWBTree(id, firstName, lastName);
    else if (id < root->id)
        root->leftWBTree = insertWBTree(root->leftWBTree, id, firstName, lastName);
    else if (id > root->id)
        root->rightWBTree = insertWBTree(root->rightWBTree, id, firstName, lastName);
    else
        return root;

    updateSize(root);
    return balanceWBTree(root);
}

void updateSize(WBTree *node) {
    if (node) {
        int leftSize = 0;
        int rightSize = 0;

        if (node->leftWBTree != NULL)
            leftSize = node->leftWBTree->size;
        if (node->rightWBTree != NULL)
            rightSize = node->rightWBTree->size;

        node->size = 1 + leftSize + rightSize;
    }
}

WBTree* balanceWBTree(WBTree *node) {
    if (!node) return NULL;

    updateSize(node);
    if (!isUnbalanced(node)) return node;

    int leftSize = 0;
    int rightSize = 0;

    if (node->leftWBTree != NULL)
        leftSize = node->leftWBTree->size;
    if (node->rightWBTree != NULL)
        rightSize = node->rightWBTree->size;

    int totalWeight = node->size;

    if (leftSize < ALPHA * totalWeight)
        node = rotateLeft(node);
    else if (rightSize < ALPHA * totalWeight)
        node = rotateRight(node);

    node->leftWBTree = balanceWBTree(node->leftWBTree);
    node->rightWBTree = balanceWBTree(node->rightWBTree);
    updateSize(node);

    return node;
}

bool isUnbalanced(WBTree *node) {
    if (!node) return false;

    int leftSize = 0;
    int rightSize = 0;

    if (node->leftWBTree != NULL)
        leftSize = node->leftWBTree->size;
    if (node->rightWBTree != NULL)
        rightSize = node->rightWBTree->size;

    int totalWeight = node->size;

    return (leftSize < ALPHA * totalWeight) || (rightSize < ALPHA * totalWeight);
}

WBTree* rotateLeft(WBTree *root) {
    if (root == NULL || root->rightWBTree == NULL) return root;
    WBTree *newRoot = root->rightWBTree;

    root->rightWBTree = newRoot->leftWBTree;
    newRoot->leftWBTree = root;

    updateSize(root);
    updateSize(newRoot);
    return newRoot;
}

WBTree* rotateRight(WBTree *root) {
    if (root == NULL || root->leftWBTree == NULL) return root;
    WBTree *newRoot = root->leftWBTree;

    root->leftWBTree = newRoot->rightWBTree;
    newRoot->rightWBTree = root;

    updateSize(root);
    updateSize(newRoot);
    return newRoot;
}

WBTree* searchWBTree(WBTree *root, unsigned int id) {
    if (root == NULL) return NULL;

    if (id < root->id)
        return searchWBTree(root->leftWBTree, id);
    else if (id > root->id)
        return searchWBTree(root->rightWBTree, id);
    else
        return root;
}

WBTree* deleteWBTree(WBTree* root, unsigned int id) {
    if (root == NULL) return NULL;

    if (id < root->id)
        root->leftWBTree = deleteWBTree(root->leftWBTree, id);
    else if (id > root->id)
        root->rightWBTree = deleteWBTree(root->rightWBTree, id);
    else {
        if (root->leftWBTree == NULL) {
            WBTree* temp = root->rightWBTree;
            free(root);
            return temp;
        } else if (root->rightWBTree == NULL) {
            WBTree* temp = root->leftWBTree;
            free(root);
            return temp;
        }

        WBTree* temp = root->rightWBTree;
        while (temp->leftWBTree != NULL) {
            temp = temp->leftWBTree;
        }

        root->id = temp->id;
        strcpy(root->firstName, temp->firstName);
        strcpy(root->lastName, temp->lastName);
        root->rightWBTree = deleteWBTree(root->rightWBTree, temp->id);
    }

    updateSize(root);
    return balanceWBTree(root);
}

void freeWBTree(WBTree* root) {
    if (root == NULL) return;

    freeWBTree(root->leftWBTree);
    freeWBTree(root->rightWBTree);
    free(root);
}
