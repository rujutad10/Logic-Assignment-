#ifndef COMMON_H
#define COMMON_H

typedef struct Node {
    char *tok;           // token string: "~", "+", "*", ">", "x1", "x12", ...
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(const char *tok);
void freeTree(Node *r);
char *strdup_s(const char *s);

#endif
