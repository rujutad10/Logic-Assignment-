/**
 * @file task4.c
 * @brief Provides functions to compute the height of a logical parse tree.
 *
 * The height is defined as the maximum number of nodes
 * along any path from the root down to a leaf node.
 *  @section algo Algorithm: Recursive height computation
 *   height(node) = max(height(left), height(right)) + 1
 * @section time Time Complexity: O(n)
 *   - Visit each node exactly once
 *   - Max operation: O(1) per node
 * @section space Space Complexity: O(h)
 *   - Recursion stack: O(h) where h = tree height
 *   - Best: O(log n) balanced, Worst: O(n) skewed
 */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Utility to return the greater of two integers.
 *
 * @param a First integer.
 * @param b Second integer.
 * @return The larger of the two values.
 */
int calcMax(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * @brief Computes the maximum height of a parse tree.
 *
 * The function recursively explores both subtrees to find their heights
 * and returns the maximum, incremented by one for the current node.
 *
 * @param root Pointer to the root node of the tree.
 * @return The height (depth) of the tree.
 */
int maxHeightOfParseTree(Node *root) {
    if (root == NULL)
        return 0;

    int leftHeight = maxHeightOfParseTree(root->left);
    int rightHeight = maxHeightOfParseTree(root->right);

    return calcMax(leftHeight, rightHeight) + 1;
}
