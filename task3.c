#include "common.h"
#include <stdio.h>
#include <string.h>

// Helper to check if a token is an operator
static int isOperator(const char* tok) {
    return (strcmp(tok, "+") == 0 || strcmp(tok, "*") == 0 ||
            strcmp(tok, ">") == 0 || strcmp(tok, "~") == 0);
}

void printInOrder(Node *root) {
    if (root == NULL) return;

    if (isOperator(root->tok)) {
        printf("(");
    }

    printInOrder(root->left);

    // Add space only if there's a token on both sides
    if (root->left) printf(" ");
    printf("%s", root->tok);
    if (root->right) printf(" ");

    printInOrder(root->right);

    if (isOperator(root->tok)) {
        printf(")");
    }
}
