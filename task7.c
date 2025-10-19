#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLAUSE_ATOMS 1024

/**
 * @brief Checks if a single clause is valid (a tautology).
 * A clause is valid if it contains both an atom and its negation (e.g., x1 and ~x1).
 * @param clause A pointer to the root node of the clause (a tree of '+' nodes).
 * @return 1 if the clause is valid, 0 otherwise.
 */
int isClauseValid(Node* clause) {
    if (!clause) return 0;

    // Arrays to store the string tokens of positive and negative literals
    char* pos_literals[MAX_CLAUSE_ATOMS];
    int pos_count = 0;
    char* neg_literals[MAX_CLAUSE_ATOMS];
    int neg_count = 0;

    // Use a stack to perform a non-recursive traversal of the clause
    Node* stack[MAX_CLAUSE_ATOMS];
    int top = -1;
    stack[++top] = clause;

    while (top >= 0) {
        Node* node = stack[top--];
        if (!node) continue;

        // Case 1: A positive literal (an atom)
        // An atom is a leaf node in the parse tree.
        if (node->left == NULL && node->right == NULL) {
            if (pos_count < MAX_CLAUSE_ATOMS) {
                pos_literals[pos_count++] = node->tok;
            }
        }
        // Case 2: A negative literal (a '~' node)
        else if (strcmp(node->tok, "~") == 0 && node->right != NULL) {
             // The operand of '~' is its right child, which must be an atom
            if (neg_count < MAX_CLAUSE_ATOMS) {
                 neg_literals[neg_count++] = node->right->tok;
            }
        }
        // Case 3: An OR operator, part of the clause structure
        else if (strcmp(node->tok, "+") == 0) {
            // Push children onto the stack to continue traversal
            if (node->left) stack[++top] = node->left;
            if (node->right) stack[++top] = node->right;
        }
    }

    // Now, compare the collected literals to find a complementary pair
    for (int i = 0; i < pos_count; i++) {
        for (int j = 0; j < neg_count; j++) {
            if (strcmp(pos_literals[i], neg_literals[j]) == 0) {
                return 1; // Found a match (e.g., "x1" in pos and "x1" in neg)
            }
        }
    }

    return 0; // No complementary pair found, clause is not a tautology
}

/**
 * @brief Traverses the CNF formula tree and counts valid and invalid clauses.
 * @param root The root of the CNF tree (a tree of '*' nodes).
 * @param validCount A pointer to an integer to store the count of valid clauses.
 * @param invalidCount A pointer to an integer to store the count of invalid clauses.
 */
void checkCNFValidity(Node* root, int *validCount, int *invalidCount) {
    if (!root) return;

    // The root of a CNF formula is a conjunction ('*') of clauses.
    // If the root is not '*', it means the formula consists of a single clause.
    if (strcmp(root->tok, "*") == 0) {
        // Recurse on the left and right subtrees (which are other conjunctions or clauses)
        checkCNFValidity(root->left, validCount, invalidCount);
        checkCNFValidity(root->right, validCount, invalidCount);
    } else {
        // If the node is not a '*', it represents a single clause.
        // We check if this clause is valid.
        if (isClauseValid(root)) {
            (*validCount)++;
        } else {
            (*invalidCount)++;
        }
    }
}
