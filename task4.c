/**
 * @brief Calculates the height of a parse tree.
 * 
 * This function recursively explores the tree, going as deep as possible into
 * both the left and right subtrees. The height at any node is determined by
 * taking the larger of the heights of its left and right subtrees, then adding 1
 * to account for the current node itself.
 * The height is computed by finding the maximum depth from the root to any leaf.
 * 
 * @param root Pointer to the root of the parse tree.
 * @return int The total height of the tree.
 */
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int calcMax(int a, int b) {
    return (a > b) ? a : b;
}

int maxHeightOfParseTree(Node *root)
{
    if (root == NULL)
        return 0; // Base case: empty tree has height 0

    int leftHeight = maxHeightOfParseTree(root->left);   // Height of left subtree
    int rightHeight = maxHeightOfParseTree(root->right); // Height of right subtree

    return calcMax(leftHeight, rightHeight) + 1; // Add 1 for the current node
}

