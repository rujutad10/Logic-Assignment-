/**
 * @file task2.c
 * @brief Builds parse tree from prefix expression and provides traversal utilities.
 * @section algo Algorithm: Recursive descent parser
 * @section time Time Complexity: O(n)
 *   - Each token processed once
 *   - One recursion per node
 * @section space Space Complexity: O(h)
 *   - Recursion stack: O(h) where h = tree height
 *   - Heap allocation: O(n) for n nodes
 *   - Best case: O(log n) for balanced trees
 *   - Worst case: O(n) for skewed trees
 */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 8192

static Node* buildTreeRecursive(char* tokens[], int* index);

/**
 * @brief Checks whether a token is a binary operator.
 * @param tok Token string.
 * @return 1 if binary operator, else 0.
 */
static int isBinaryOperator(const char* tok) {
    return (strcmp(tok, "+") == 0 || strcmp(tok, "*") == 0 || strcmp(tok, ">") == 0);
}

/**
 * @brief Converts prefix string to a parse tree.
 * @param root Pointer to root node pointer.
 * @param prefixString Input prefix string.
 */
void convertPreOrderToTree(Node **root, char *prefixString) {
    char* tokens[MAX_TOKENS];
    int tokenCount = 0;

    char* buffer = strdup_s(prefixString);
    char* tok = strtok(buffer, " \t\n");
    while (tok != NULL && tokenCount < MAX_TOKENS) {
        tokens[tokenCount++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    tokens[tokenCount] = NULL;

    int index = 0;
    *root = buildTreeRecursive(tokens, &index);

    free(buffer);
}

/**
 * @brief Recursively builds parse tree from prefix tokens.
 * @param tokens Array of tokens.
 * @param index Pointer to current token index.
 * @return Pointer to the newly constructed node.
 */
static Node* buildTreeRecursive(char* tokens[], int* index) {
    char* currentToken = tokens[*index];
    (*index)++;

    if (currentToken == NULL) return NULL;

    Node* node = newNode(currentToken);

    if (strcmp(currentToken, "~") == 0) {
        node->left = NULL;
        node->right = buildTreeRecursive(tokens, index);
    } else if (isBinaryOperator(currentToken)) {
        node->left = buildTreeRecursive(tokens, index);
        node->right = buildTreeRecursive(tokens, index);
    }
    return node;
}

/**
 * @brief Prints parse tree in preorder.
 * @param root Root node pointer.
 */
void printPreOrder(Node *root) {
    if (root == NULL) return;
    printf("%s ", root->tok);
    printPreOrder(root->left);
    printPreOrder(root->right);
}
/**
 * @brief Prints parse tree in inorder (with parentheses).
 * @param root Root node pointer.
 */
void printInOrder(Node *root) {
    if (root == NULL) return;

    // If the node is an operator, add parentheses
    int isOperator = (root->left != NULL || root->right != NULL);
    if (isOperator) printf("(");

    printInOrder(root->left);
    printf("%s", root->tok);
    printInOrder(root->right);

    if (isOperator) printf(")");
}

/**
 * @brief Prints parse tree in postorder.
 * @param root Root node pointer.
 */
void printPostOrder(Node *root) {
    if (root == NULL) return;
    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%s ", root->tok);
}
