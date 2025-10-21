/**
 * @file task2.h
 * @brief Header for prefix-to-parse-tree conversion and traversal functions.
 */

#ifndef TASK2_H
#define TASK2_H

#include "common.h"

/**
 * @brief Converts prefix expression string into a parse tree.
 * @param root Pointer to root node pointer.
 * @param start Input prefix string.
 */
void convertPreOrderToTree(Node **root, char *start);

/**
 * @brief Prints preorder traversal of parse tree.
 * @param root Root node.
 */
void printPreOrder(Node *root);

/**
 * @brief Prints postorder traversal of parse tree.
 * @param root Root node.
 */
void printPostOrder(Node *root);
void printInOrder(Node *root); // add this

#endif
