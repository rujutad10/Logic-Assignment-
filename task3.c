#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int noOfCharacters = 0;

void printInOrder(Node *root) {
    if (root == NULL) return;

    // Check if it's an operator (i.e., not a leaf)
    if (root->left != NULL || root->right != NULL) {
        printf("(");
    }
    
    printInOrder(root->left);
    printf("%s ", root->tok); // Print the string token
    printInOrder(root->right);

    if (root->left != NULL || root->right != NULL) {
        printf(")");
    }
}
