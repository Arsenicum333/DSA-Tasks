#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHA 0.25 // Balance factor for Tree

typedef struct WBTree {
    unsigned int id;
    char firstName[20];
    char lastName[20];
    unsigned int size; // Number of nodes in Subtree
    struct WBTree* leftWBTree;
    struct WBTree* rightWBTree;
} WBTree;

WBTree* createWBTree(unsigned int id, const char *firstName, const char *lastName);
WBTree* insertWBTree(WBTree *root, unsigned int id, const char *firstName, const char *lastName);
void updateSize(WBTree *node);
WBTree* balanceWBTree(WBTree *node);
bool isUnbalanced(WBTree *node);
WBTree* rotateLeftWBTree(WBTree *root);
WBTree* rotateRightWBTree(WBTree *root);
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

    if (id < root->id)
        root->leftWBTree = insertWBTree(root->leftWBTree, id, firstName, lastName);
    else if (id > root->id)
        root->rightWBTree = insertWBTree(root->rightWBTree, id, firstName, lastName);
    else
        return root; // If duplicate, skip insertion

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
    if (node == NULL) return NULL;

    updateSize(node);
    if (!isUnbalanced(node)) return node; // No balancing needed if tree is balanced

    int leftSize = 0;
    int rightSize = 0;

    if (node->leftWBTree != NULL)
        leftSize = node->leftWBTree->size;
    if (node->rightWBTree != NULL)
        rightSize = node->rightWBTree->size;

    int totalWeight = node->size;

    // Rotate if Subtree is less than 25% of total weight
    if (leftSize < ALPHA * totalWeight)
        node = rotateLeftWBTree(node);
    else if (rightSize < ALPHA * totalWeight)
        node = rotateRightWBTree(node);

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

    // Check if either Subtree is too small compared to total weight
    return (leftSize < ALPHA * totalWeight) || (rightSize < ALPHA * totalWeight);
}

WBTree* rotateLeftWBTree(WBTree *root) {
    if (root == NULL || root->rightWBTree == NULL) return root;

    WBTree *newRoot = root->rightWBTree; // Right child becomes New Root
    root->rightWBTree = newRoot->leftWBTree; // Move New Root left to Old Root right
    newRoot->leftWBTree = root; // Move Old Root to New Root left Subtree

    updateSize(root); // First update Old Root
    updateSize(newRoot); // Then update New Root
    return newRoot;
}

WBTree* rotateRightWBTree(WBTree *root) {
    if (root == NULL || root->leftWBTree == NULL) return root;

    WBTree *newRoot = root->leftWBTree; // Left child becomes New Root
    root->leftWBTree = newRoot->rightWBTree; // Move New Root right to Old Root left
    newRoot->rightWBTree = root; // Move Old Root to New Root right Subtree

    updateSize(root); // First update Old Root
    updateSize(newRoot); // Then update New Root
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
        // Case 1: No children - remove directly
        if (root->leftWBTree == NULL && root->rightWBTree == NULL) {
            free(root);
            return NULL;
        }
        // Case 2: No left child - replace with right subtree
        if (root->leftWBTree == NULL) {
            WBTree* temp = root->rightWBTree;
            free(root);
            return temp;
        }
        // Case 3: No right child - replace with left subtree
        else if (root->rightWBTree == NULL) {
            WBTree* temp = root->leftWBTree;
            free(root);
            return temp;
        }
        // Case 4: Two children - replace with successor (smallest in right subtree)
        WBTree* temp = root->rightWBTree;
        while (temp->leftWBTree != NULL)
            temp = temp->leftWBTree; // Find smallest node in right subtree

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
