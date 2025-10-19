// common.h
#ifndef COMMON_H
#define COMMON_H


typedef struct Node {
    char *tok; // This holds a STRING, not a char
    struct Node *left;
    struct Node *right;
} Node;

// Helper function prototypes
Node* newNode(const char *tok);
void freeTree(Node *r);
char *strdup_s(const char *s);

#endif
