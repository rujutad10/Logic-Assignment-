#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 8192 // Increased for larger CNF files

// Forward declaration
Node* buildTreeRecursive(char* tokens[], int* index);

// Helper function to check if a token is a binary operator
static int isBinaryOperator(const char* tok) {
    return (strcmp(tok, "+") == 0 || strcmp(tok, "*") == 0 || strcmp(tok, ">") == 0);
}

/**
 * @brief Main wrapper function to convert a prefix string into a parse tree.
 * @param root A pointer to the Node pointer that will become the root.
 * @param prefixString The space-separated prefix formula string.
 */
void convertPreOrderToTree(Node **root, char *prefixString) {
    char* tokens[MAX_TOKENS];
    int tokenCount = 0;

    // Tokenize the prefix string (it's separated by spaces)
    char* buffer = strdup_s(prefixString); // Work on a copy
    char* tok = strtok(buffer, " \t\n");
    while (tok != NULL && tokenCount < MAX_TOKENS) {
        tokens[tokenCount++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    tokens[tokenCount] = NULL; // Null-terminate the array of tokens

    int index = 0; // Start building from the first token
    *root = buildTreeRecursive(tokens, &index);

    free(buffer); // Clean up the copied string
}

/**
 * @brief The core recursive function that builds the tree from tokens.
 * @param tokens An array of string tokens in prefix order.
 * @param index A pointer to the current position in the token array.
 * @return A pointer to the root of the newly constructed subtree.
 */
Node* buildTreeRecursive(char* tokens[], int* index) {
    // Get the current token and advance the index for the next recursive call
    char* currentToken = tokens[*index];
    (*index)++;

    if (currentToken == NULL) {
        return NULL;
    }

    // Create the new node for the current token
    Node* node = newNode(currentToken);

    // If the token is a negation operator, it has one child (right)
    if (strcmp(currentToken, "~") == 0) {
        node->left = NULL;
        node->right = buildTreeRecursive(tokens, index);
        return node;
    }
    // If the token is a binary operator, it has two children
    else if (isBinaryOperator(currentToken)) {
        node->left = buildTreeRecursive(tokens, index);
        node->right = buildTreeRecursive(tokens, index);
        return node;
    }
    // Otherwise, it's an atom (a leaf node)
    else {
        node->left = NULL;
        node->right = NULL;
        return node;
    }
}

// --- Tree Traversal Functions (for testing/output) ---

void printPreOrder(Node *root) {
    if (root == NULL) return;
    printf("%s ", root->tok);
    printPreOrder(root->left);
    printPreOrder(root->right);
}

void printPostOrder(Node *root) {
    if (root == NULL) return;
    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%s ", root->tok);
}
