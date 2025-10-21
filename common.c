#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @copydoc strdup_s
 */
char *strdup_s(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    if (!r) { perror("malloc"); return NULL; }
    strcpy(r, s);
    return r;
}

/**
 * @copydoc newNode
 */
Node* newNode(const char *tok) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) { perror("malloc"); return NULL; }
    n->tok = strdup_s(tok);
    n->left = NULL;
    n->right = NULL;
    return n;
}

/**
 * @copydoc freeTree
 */
void freeTree(Node *r) {
    if (!r) return;
    freeTree(r->left);
    freeTree(r->right);
    if (r->tok) free(r->tok);
    free(r);
}
