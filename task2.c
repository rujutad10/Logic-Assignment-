#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_TOKENS 1024

// --- Helper function to check for atoms ---
int isAtom(char* tok) {
    if (strcmp(tok, "+") != 0 && strcmp(tok, "*") != 0 &&
        strcmp(tok, ">") != 0 && strcmp(tok, "~") != 0) {
        return 1;
    }
    return 0;
}

// --- Recursive builder function ---
// Takes an array of tokens and a pointer to the current index
Node* buildTreeRecursive(char* tokens[], int* index) {
    // Get the current token and advance the index
    char* currentToken = tokens[(*index)++];
    if (currentToken == NULL) {
        return NULL;
    }

    // Create the new node using the function from common.c
    Node* node = newNode(currentToken);

    if (isAtom(currentToken)) {
        // Atoms are leaves, no children
        node->left = NULL;
        node->right = NULL;
        return node;
    }

    if (strcmp(currentToken, "~") == 0) {
        // Negation has one child (right)
        node->left = NULL;
        node->right = buildTreeRecursive(tokens, index);
        return node;
    }

    // All others are binary operators (+, *, >)
    node->left = buildTreeRecursive(tokens, index);
    node->right = buildTreeRecursive(tokens, index);
    return node;
}

// --- Main convertPreOrderToTree function ---
void convertPreOrderToTree(Node **root, char *prefixString) {
    char* tokens[MAX_TOKENS];
    int tokenCount = 0;
    
    // Tokenize the prefix string (it's space-separated)
    char* tok = strtok(prefixString, " \t\n");
    while (tok != NULL && tokenCount < MAX_TOKENS) {
        tokens[tokenCount++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    tokens[tokenCount] = NULL; // Null-terminate the array

    int index = 0;
    *root = buildTreeRecursive(tokens, &index);
}

void printInOrder(Node *root);
void printPreOrder(Node *root);
void printPostOrder(Node *root);

// ---------------------------------------------
// Tree Traversal Functions
// ---------------------------------------------

void printPreOrder(Node *root)
{
    if (root == NULL)
        return;
    printf("%c", root->val);
    printPreOrder(root->left);
    printPreOrder(root->right);
}

void printPostOrder(Node *root)
{
    if (root == NULL)
        return;
    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%c", root->val);
}

// ---------------------------------------------
// Main Function for Testing
// ---------------------------------------------
/*int main()
{
    char prefixExpr[100];

    printf("Enter Prefix Expression: ");
    scanf("%s", prefixExpr);

    Node *root = NULL;
    convertPreOrderToTree(&root, prefixExpr);

    printf("\nInfix Expression   : ");
    printInOrder(root);
    printf("\nPrefix Expression  : ");
    printPreOrder(root);
    printf("\nPostfix Expression : ");
    printPostOrder(root);
    printf("\n");
    return 0;
}*/
