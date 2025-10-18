#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "task2.h"
#include "task2.h"

/*// ---------------------------------------------
// Tree Node Structure
// ---------------------------------------------
typedef struct TreeNode
{
    char val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;*/

// ---------------------------------------------
// Function Declaration
// ---------------------------------------------
char *convertPreOrderToTree(TreeNode **root, char *start);
void printInOrder(TreeNode *root);
void printPreOrder(TreeNode *root);
void printPostOrder(TreeNode *root);

// ---------------------------------------------
// Recursive Function: Convert Prefix to Expression Tree
// ---------------------------------------------
char *convertPreOrderToTree(TreeNode **root, char *start)
{
    // Base condition
    if (*start == '\0')
        return start;

    // Create new node if root is NULL
    if (*root == NULL)
    {
        TreeNode *newTreeNode = (TreeNode *)malloc(sizeof(TreeNode));
        newTreeNode->val = *start;
        newTreeNode->left = NULL;
        newTreeNode->right = NULL;
        *root = newTreeNode;
    }

    // If current char is operand (A, B, C, etc.) → no children
    if (isalpha(*start))
    {
        return start;
    }

    // If unary operator (~)
    if (*start == '~')
    {
        char *currentLocation = convertPreOrderToTree(&(*root)->right, start + 1);
        return currentLocation;
    }

    // Otherwise, it’s a binary operator (+, *, >, etc.)
    char *currentLocation = convertPreOrderToTree(&(*root)->left, start + 1);
    currentLocation = convertPreOrderToTree(&(*root)->right, currentLocation + 1);
    return currentLocation;
}

// ---------------------------------------------
// Tree Traversal Functions
// ---------------------------------------------
void printInOrder(TreeNode *root)
{
    if (root == NULL)
        return;
    if (root->left != NULL)
        printf("(");
    printInOrder(root->left);
    printf("%c", root->val);
    printInOrder(root->right);
    if (root->right != NULL)
        printf(")");
}

void printPreOrder(TreeNode *root)
{
    if (root == NULL)
        return;
    printf("%c", root->val);
    printPreOrder(root->left);
    printPreOrder(root->right);
}

void printPostOrder(TreeNode *root)
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

    TreeNode *root = NULL;
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
