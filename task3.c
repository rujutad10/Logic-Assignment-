/**
 * @file task3.c
 * @brief Implements the inorder traversal for printing parse trees.
 *
 * This module provides the inorder traversal logic used to display
 * the logical formula in its original infix (human-readable) format.
 * @section algo Algorithm: Recursive tree traversal
 * @section time Time Complexity: O(n)
 *   - Visit each node exactly once
 *   - Print operation: O(1) per node
 * @section space Space Complexity: O(h)
 *   - Recursion call stack: O(h) where h = tree height
 *   - No additional data structures
 */

#include "common.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Checks whether the given token is a logical operator.
 *
 * @param tok The token string (e.g., "+", "*", ">", "~").
 * @return 1 if the token is an operator, 0 otherwise.
 */
static int isOperator(const char* tok) {
    return (strcmp(tok, "+") == 0 ||
            strcmp(tok, "*") == 0 ||
            strcmp(tok, ">") == 0 ||
            strcmp(tok, "~") == 0);
}

/**
 * @brief Prints the parse tree in inorder (infix) notation.
 *
 * Recursively traverses the tree, enclosing operators within parentheses
 * to preserve logical precedence. Produces output resembli*
**/