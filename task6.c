#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations for recursive functions
Node* eliminateImplications(Node* root);
Node* moveNotInwards(Node* root);
Node* distributeOr(Node* root);
Node* copyTree(Node* root);

/**
 * @brief Utility function to create a new node.
 * This is a simple wrapper around the newNode function from common.c.
 * @param tok The string token for the node's value (e.g., "+", "~", "x12").
 * @return A pointer to the newly created Node.
 */
Node* createNode(const char* tok) {
    return newNode(tok);
}

/**
 * @brief Creates a deep copy of a parse tree.
 * @param root The root of the tree to copy.
 * @return A pointer to the root of the new, copied tree.
 */
Node* copyTree(Node* root) {
    if (!root) return NULL;
    // Create a new node with a copy of the token string
    Node* newNode = createNode(root->tok);
    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);
    return newNode;
}

/**
 * @brief Step 1: Recursively eliminates all implication operators ('>').
 * Replaces every (A > B) with (~A + B).
 * @param root The root of the tree/subtree to process.
 * @return The root of the modified tree.
 */
Node* eliminateImplications(Node* root) {
    if (!root) return NULL;

    // Recurse down to the leaves first
    root->left = eliminateImplications(root->left);
    root->right = eliminateImplications(root->right);

    // If we find an implication node, transform it
    if (strcmp(root->tok, ">") == 0) {
        // Change current node's value from ">" to "+"
        free(root->tok);
        root->tok = strdup_s("+");

        // Create a new negation node '~'
        Node* notLeft = createNode("~");
        notLeft->right = root->left; // The operand for '~' is its right child
        notLeft->left = NULL;

        // Replace the original left child with the new negation structure
        root->left = notLeft;
    }
    return root;
}

/**
 * @brief Step 2: Recursively moves all negation operators ('~') inward.
 * Applies De Morgan's laws and eliminates double negations (~~A).
 * @param root The root of the implication-free tree.
 * @return The root of the modified tree in Negation Normal Form (NNF).
 */
Node* moveNotInwards(Node* root) {
    if (!root) return NULL;

    // We only need to act if the current node is a negation
    if (strcmp(root->tok, "~") == 0) {
        Node* child = root->right; // The operand of ~ is the right child

        // Case 1: Double Negation (~~A becomes A)
        if (strcmp(child->tok, "~") == 0) {
            Node* grandChild = child->right;
            free(root->tok); // Free the outer '~' token
            free(root);      // Free the outer '~' node
            free(child->tok); // Free the inner '~' token
            free(child);      // Free the inner '~' node
            // Recurse on the grandchild, as there could be more negations (e.g., ~~~~A)
            return moveNotInwards(grandChild);
        }

        // Case 2: De Morgan's Law for AND: ~(A * B) becomes (~A + ~B)
        if (strcmp(child->tok, "*") == 0) {
            // Change the root node from '~' to '+'
            free(root->tok);
            root->tok = strdup_s("+");

            // Create new negation nodes for the original children
            Node* notLeft = createNode("~");
            notLeft->right = child->left; // Note: child of ~ is always right

            Node* notRight = createNode("~");
            notRight->right = child->right;

            // The root's new children are the recursed-upon negations
            root->left = moveNotInwards(notLeft);
            root->right = moveNotInwards(notRight);
            
            free(child->tok); // Free the '*' token
            free(child);      // Free the '*' node
            return root;
        }

        // Case 3: De Morgan's Law for OR: ~(A + B) becomes (~A * ~B)
        if (strcmp(child->tok, "+") == 0) {
            // Change the root node from '~' to '*'
            free(root->tok);
            root->tok = strdup_s("*");

            Node* notLeft = createNode("~");
            notLeft->right = child->left;
            
            Node* notRight = createNode("~");
            notRight->right = child->right;

            root->left = moveNotInwards(notLeft);
            root->right = moveNotInwards(notRight);

            free(child->tok); // Free the '+' token
            free(child);      // Free the '+' node
            return root;
        }
    }

    // If not a '~' node, just recurse on children
    root->left = moveNotInwards(root->left);
    root->right = moveNotInwards(root->right);
    return root;
}


/**
 * @brief Step 3: Recursively distributes OR ('+') over AND ('*').
 * Replaces A + (B * C) with (A + B) * (A + C).
 * @param root The root of the tree in NNF.
 * @return The root of the final CNF tree.
 */
Node* distributeOr(Node* root) {
    if (!root) return NULL;

    // Recurse down to the leaves first
    root->left = distributeOr(root->left);
    root->right = distributeOr(root->right);

    // Check if the current node is an OR
    if (strcmp(root->tok, "+") == 0) {
        // Case 1: A + (B * C)
        if (root->right && strcmp(root->right->tok, "*") == 0) {
            Node* a = root->left;
            Node* b = root->right->left;
            Node* c = root->right->right;

            // Create the new structure: (A + B) * (A + C)
            Node* newLeft = createNode("+");
            newLeft->left = copyTree(a);
            newLeft->left = distributeOr(newLeft->left);
            newLeft->right = b;

            Node* newRight = createNode("+");
            newRight->left = a;
            newRight->right = c;

            // Change the original root node from '+' to '*'
            free(root->tok);
            root->tok = strdup_s("*");
            root->left = distributeOr(newLeft);
            root->right = distributeOr(newRight);

            // Free the old '*' node that was replaced
            free(root->right->tok);
            free(root->right);
        }
        // Case 2: (A * B) + C
        else if (root->left && strcmp(root->left->tok, "*") == 0) {
            Node* a = root->left->left;
            Node* b = root->left->right;
            Node* c = root->right;
            
            // Create the new structure: (A + C) * (B + C)
            Node* newLeft = createNode("+");
            newLeft->left = a;
            newLeft->right = copyTree(c);
            newLeft->right = distributeOr(newLeft->right);

            Node* newRight = createNode("+");
            newRight->left = b;
            newRight->right = c;
            
            // Change the original root node from '+' to '*'
            free(root->tok);
            root->tok = strdup_s("*");
            root->left = distributeOr(newLeft);
            root->right = distributeOr(newRight);
            
            // Free the old '*' node that was replaced
            free(root->left->tok);
            free(root->left);
        }
    }
    return root;
}

/**
 * @brief Wrapper function that performs the full conversion to CNF.
 * @param root The root of the original parse tree.
 * @return A pointer to the root of the new tree in CNF.
 */
Node* convertToCNF(Node* root) {
    // We operate on a copy to preserve the original tree
    Node* cnfTree = copyTree(root);
    cnfTree = eliminateImplications(cnfTree);
    cnfTree = moveNotInwards(cnfTree);
    cnfTree = distributeOr(cnfTree);
    return cnfTree;
}

/**
 * @brief Utility function to print the CNF formula.
 * @param root The root of the CNF tree.
 */
void printCNF(Node* root) {
    if (!root) return;
    
    // Add parentheses around binary operations
    if (strcmp(root->tok, "*") == 0 || strcmp(root->tok, "+") == 0) {
        printf("(");
    }
    
    printCNF(root->left);
    printf("%s ", root->tok); // Print the string token with a space
    printCNF(root->right);
    
    if (strcmp(root->tok, "*") == 0 || strcmp(root->tok, "+") == 0) {
        printf(")");
    }
}

