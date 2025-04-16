#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Treap {
    unsigned int id;
    char firstName[20];
    char lastName[20];
    unsigned int priority; // Random priority for heap property
    struct Treap* leftTreap;
    struct Treap* rightTreap;
} Treap;

Treap* createTreap(unsigned int id, const char *firstName, const char *lastName);
Treap* insertTreap(Treap *root, unsigned int id, const char *firstName, const char *lastName);
Treap* rotateLeftTreap(Treap *root);
Treap* rotateRightTreap(Treap *root);
Treap* searchTreap(Treap *root, unsigned int id);
Treap* deleteTreap(Treap* root, unsigned int id);
int countTreapNodes(Treap* root);
void freeTreap(Treap* root);

Treap* createTreap(unsigned int id, const char *firstName, const char *lastName) {
    Treap *node = (Treap*)calloc(1, sizeof(Treap));
    node->id = id;
    strcpy(node->firstName, firstName);
    strcpy(node->lastName, lastName);
    node->priority = rand(); // Assign random priority for balancing
    node->leftTreap = NULL;
    node->rightTreap = NULL;
    return node;
}

Treap* insertTreap(Treap *root, unsigned int id, const char *firstName, const char *lastName) {
    if (root == NULL)
        return createTreap(id, firstName, lastName);

    if (id < root->id) {
        root->leftTreap = insertTreap(root->leftTreap, id, firstName, lastName);

        // Rotate right if left child's priority exceeds parent's
        if (root->leftTreap->priority > root->priority)
            root = rotateRightTreap(root);
    } else if (id > root->id) {
        root->rightTreap = insertTreap(root->rightTreap, id, firstName, lastName);

        // Rotate left if right child's priority exceeds parent's
        if (root->rightTreap->priority > root->priority)
            root = rotateLeftTreap(root);
    } else
        return root; // If duplicate, skip insertion

    return root;
}

Treap* rotateLeftTreap(Treap *root) {
    if (root == NULL || root->rightTreap == NULL) return root;

    Treap *newRoot = root->rightTreap; // Right child becomes New Root
    root->rightTreap = newRoot->leftTreap; // Move New Root left to Old Root right
    newRoot->leftTreap = root; // Move Old Root to New Root left Subtree

    return newRoot;
}

Treap* rotateRightTreap(Treap *root) {
    if (root == NULL || root->leftTreap == NULL) return root;

    Treap *newRoot = root->leftTreap; // Left child becomes New Root
    root->leftTreap = newRoot->rightTreap; // Move New Root right to Old Root left
    newRoot->rightTreap = root; // Move Old Root to New Root right Subtree

    return newRoot;
}

Treap* searchTreap(Treap *root, unsigned int id) {
    if (root == NULL) return NULL;

    if (id < root->id)
        return searchTreap(root->leftTreap, id);
    else if (id > root->id)
        return searchTreap(root->rightTreap, id);
    else
        return root;
}

Treap* deleteTreap(Treap* root, unsigned int id) {
    if (root == NULL) return NULL;

    if (id < root->id)
        root->leftTreap = deleteTreap(root->leftTreap, id);
    else if (id > root->id)
        root->rightTreap = deleteTreap(root->rightTreap, id);
    else {
        // Case 1: No children - remove directly
        if (root->leftTreap == NULL && root->rightTreap == NULL) {
            free(root);
            return NULL;
        }
        // Case 2: Only right child - replace with right
        else if (root->leftTreap == NULL) {
            Treap* temp = root->rightTreap;
            free(root);
            return temp;
        }
        // Case 3: Only left child - replace with left
        else if (root->rightTreap == NULL) {
            Treap* temp = root->leftTreap;
            free(root);
            return temp;
        }
        // Case 4: Two children - rotate based on priority and recurse
        else {
            if (root->leftTreap->priority > root->rightTreap->priority) {
                root = rotateRightTreap(root); // Move left child up
                root->rightTreap = deleteTreap(root->rightTreap, id); // Continue deletion
            } else {
                root = rotateLeftTreap(root); // Move right child up
                root->leftTreap = deleteTreap(root->leftTreap, id); // Continue deletion
            }
        }
    }

    return root;
}

void freeTreap(Treap* root) {
    if (root == NULL) return;

    freeTreap(root->leftTreap);
    freeTreap(root->rightTreap);
    free(root);
}
