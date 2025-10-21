/**
 * @file task6.c
 * @brief Implements logical formula conversion to Conjunctive Normal Form (CNF).
 *
 * This module eliminates implications, applies De Morgan’s laws,
 * and distributes OR over AND to obtain a valid CNF representation.
 * @section algo Algorithm:
 *   Step 1: Eliminate implications (A > B) → (~A + B)
 *   Step 2: Move negations inward (De Morgan's laws)
 *   Step 3: Distribute OR over AND (convert to CNF)
 * 
 * @section time Time Complexity: O(2^n) worst case
 *   - Step 1 (Eliminate >): O(n) single pass
 *   - Step 2 (Move ~): O(n) single pass
 *   - Step 3 (Distribute +): O(n × k) tree expansion
 *     k = expansion factor, can be exponential
 *   - WORST CASE: Tree expands from O(n) to O(2^n) nodes
 * 
 * @section space Space Complexity: O(2^n) worst case
 *   - Original tree copy: O(n)
 *   - Expanded tree: O(2^n) in worst case
 *   - Multiple intermediate trees during transformation
 */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
Node* eliminateImplications(Node* root);
Node* moveNotInwards(Node* root);
Node* distributeOr(Node* root);
Node* copyTree(Node* root);

/**
 * @brief Creates a deep copy of a parse tree.
 *
 * @param root Root node of the original tree.
 * @return Pointer to the new tree.
 */
Node* copyTree(Node* root) {
    if (!root) return NULL;

    Node* nodeCopy = newNode(root->tok);  // ✅ no naming conflict now
    nodeCopy->left = copyTree(root->left);
    nodeCopy->right = copyTree(root->right);

    return nodeCopy;
}


/**
 * @brief Eliminates implication operators (A > B → (~A + B)).
 *
 * @param root Root node of the tree/subtree.
 * @return Modified tree root.
 */
Node* eliminateImplications(Node* root) {
    if (!root) return NULL;
    root->left = eliminateImplications(root->left);
    root->right = eliminateImplications(root->right);

    if (strcmp(root->tok, ">") == 0) {
        free(root->tok);
        root->tok = strdup_s("+");
        Node* notLeft = newNode("~");
        notLeft->right = root->left;
        root->left = notLeft;
    }
    return root;
}

/**
 * @brief Pushes negations inward using De Morgan’s laws.
 *
 * Handles double negations and distributes negations across operators.
 *
 * @param root Root node.
 * @return Tree in Negation Normal Form (NNF).
 */
Node* moveNotInwards(Node* root) {
    if (!root) return NULL;

    if (strcmp(root->tok, "~") == 0) {
        Node* child = root->right;
        if (strcmp(child->tok, "~") == 0)
            return moveNotInwards(child->right);
        if (strcmp(child->tok, "*") == 0) {
            root->tok = strdup_s("+");
            Node* a = newNode("~"); a->right = child->left;
            Node* b = newNode("~"); b->right = child->right;
            root->left = moveNotInwards(a);
            root->right = moveNotInwards(b);
            return root;
        }
        if (strcmp(child->tok, "+") == 0) {
            root->tok = strdup_s("*");
            Node* a = newNode("~"); a->right = child->left;
            Node* b = newNode("~"); b->right = child->right;
            root->left = moveNotInwards(a);
            root->right = moveNotInwards(b);
            return root;
        }
    }
    root->left = moveNotInwards(root->left);
    root->right = moveNotInwards(root->right);
    return root;
}

/**
 * @brief Distributes OR (‘+’) over AND (‘*’) recursively.
 *
 * @param root Root of the NNF tree.
 * @return Tree converted into CNF.
 */
Node* distributeOr(Node* root) {
    if (!root) return NULL;
    root->left = distributeOr(root->left);
    root->right = distributeOr(root->right);

    if (strcmp(root->tok, "+") == 0) {
        if (root->right && strcmp(root->right->tok, "*") == 0) {
            Node* a = root->left, *b = root->right->left, *c = root->right->right;
            Node* left = newNode("+"); left->left = copyTree(a); left->right = b;
            Node* right = newNode("+"); right->left = a; right->right = c;
            root->tok = strdup_s("*");
            root->left = distributeOr(left);
            root->right = distributeOr(right);
        }
    }
    return root;
}

/**
 * @brief Converts a formula tree to Conjunctive Normal Form.
 *
 * @param root Root of the original formula tree.
 * @return Root of the CNF tree.
 */
Node* convertToCNF(Node* root) {
    Node* cnfTree = copyTree(root);
    cnfTree = eliminateImplications(cnfTree);
    cnfTree = moveNotInwards(cnfTree);
    cnfTree = distributeOr(cnfTree);
    return cnfTree;
}

/**
 * @brief Prints the CNF formula in infix form.
 *
 * @param root Root node of the CNF tree.
 */
void printCNF(Node* root) {
    if (!root) return;
    if (strcmp(root->tok, "*") == 0 || strcmp(root->tok, "+") == 0) printf("(");
    printCNF(root->left);
    printf(" %s ", root->tok);
    printCNF(root->right);
    if (strcmp(root->tok, "*") == 0 || strcmp(root->tok, "+") == 0) printf(")");
}
