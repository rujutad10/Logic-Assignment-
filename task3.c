#include "common.h"

int noOfCharacters = 0;

void printInOrder(TreeNode *root)
{
    if (root == NULL)
        return;

    // If the node is an operator, print '('
    if (!isalpha(root->val))
        printf("(");

    // Recurse left
    printInOrder(root->left);

    // Print current node's value
    printf("%c", root->val);

    // Recurse right
    printInOrder(root->right);

    // If the node is an operator, print ')'
    if (!isalpha(root->val))
        printf(")");
}
