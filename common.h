#ifndef COMMON_H
#define COMMON_H

/**
 * @brief Represents a node in a parse tree.
 *
 * Each node contains a string token and pointers to its left and right children.
 */
typedef struct Node {
    char *tok;        /**< Token stored in this node (string, not single char) */
    struct Node *left;  /**< Pointer to left child node */
    struct Node *right; /**< Pointer to right child node */
} Node;

/**
 * @brief Creates a new Node with a given token.
 * @param tok The string token for the node.
 * @return Pointer to the newly created Node. Returns NULL on malloc failure.
 */
Node* newNode(const char *tok);

/**
 * @brief Frees all nodes in a tree rooted at r.
 * @param r Pointer to the root node.
 */
void freeTree(Node *r);

/**
 * @brief Duplicates a string safely (malloc + strcpy).
 * @param s Input string.
 * @return Pointer to newly allocated copy. NULL if input is NULL or malloc fails.
 */
char *strdup_s(const char *s);

#endif
