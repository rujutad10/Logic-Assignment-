/**
 * @file task7.c
 * @brief Provides functionality to check CNF clause validity.
 *
 * This module checks if clauses within a CNF formula are tautologies,
 * i.e., they contain both a literal and its negation.
 * @section algo Algorithm: Single-pass tree traversal
 *   Collect positive and negative literals per clause
 *   Check for complementary pairs (x and ~x)
 * 
 * @section time Time Complexity: O(n)
 *   - n = total nodes in CNF tree
 *   - m = number of clauses, c = avg clause size
 *   - Overall: O(m × c) = O(n)
 *   - Single traversal with stack-based literal collection
 * 
 * @section space Space Complexity: O(c)
 *   - c = maximum clause size
 *   - Literal arrays: O(c)
 *   - Stack for traversal: O(c)
 *   - No exponential blow-up like Task 6
 */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLAUSE_ATOMS 1024

/**
 * @brief Checks whether a single CNF clause is valid.
 *
 * A clause is valid if it contains both a variable and its negation
 * (e.g., A and ~A). Such clauses always evaluate to True.
 *
 * @param clause Root node of the clause subtree.
 * @return 1 if valid, 0 otherwise.
 */
int isClauseValid(Node* clause) {
    if (!clause) return 0;

    char* pos[MAX_CLAUSE_ATOMS];
    char* neg[MAX_CLAUSE_ATOMS];
    int posCount = 0, negCount = 0;

    Node* stack[MAX_CLAUSE_ATOMS];
    int top = -1;
    stack[++top] = clause;

    while (top >= 0) {
        Node* node = stack[top--];
        if (!node) continue;

        if (node->left == NULL && node->right == NULL)
            pos[posCount++] = node->tok;
        else if (strcmp(node->tok, "~") == 0 && node->right)
            neg[negCount++] = node->right->tok;
        else if (strcmp(node->tok, "+") == 0) {
            if (node->left) stack[++top] = node->left;
            if (node->right) stack[++top] = node->right;
        }
    }

    for (int i = 0; i < posCount; i++)
        for (int j = 0; j < negCount; j++)
            if (strcmp(pos[i], neg[j]) == 0)
                return 1;

    return 0;
}

/**
 * @brief Recursively counts valid and invalid clauses in a CNF formula.
 *
 * @param root Root node of the CNF formula tree.
 * @param validCount Output counter for valid (tautological) clauses.
 * @param invalidCount Output counter for invalid (non-tautological) clauses.
 */
void checkCNFValidity(Node* root, int *validCount, int *invalidCount) {
    if (!root) return;

    if (strcmp(root->tok, "*") == 0) {
        checkCNFValidity(root->left, validCount, invalidCount);
        checkCNFValidity(root->right, validCount, invalidCount);
    } else {
        if (isClauseValid(root)) (*validCount)++;
        else (*invalidCount)++;
    }
}
