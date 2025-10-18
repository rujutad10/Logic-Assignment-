#include "task2.h"

int noOfCharacters = 0;

void printInOrder(TreeNode *root)
{
    if (root == NULL)
        return;
    // A new statement to figure out the number of characters
    if (isalpha(root->val))
        noOfCharacters++;
    // Goes to the left sub tree
    printInOrder(root->left);
    // Prints current value
    printf("%c", root->val);
    // Goes to the right sub tree
    printInOrder(root->right);
}