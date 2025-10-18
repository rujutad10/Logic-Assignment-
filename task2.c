#include "task2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// tokenize prefix by spaces
static char **tokenize(const char *s, int *cnt) {
    char *copy = strdup(s);
    int cap = 256;
    char **arr = malloc(sizeof(char*) * cap);
    int c = 0;
    char *tok = strtok(copy, " \t\r\n");
    while (tok) {
        if (c >= cap) { cap *= 2; arr = realloc(arr, sizeof(char*) * cap); }
        arr[c++] = strdup(tok);
        tok = strtok(NULL, " \t\r\n");
    }
    free(copy);
    *cnt = c;
    return arr;
}

static int idx;
static char **toks;
static int toks_n;

static int isOpTok(const char *s) {
    return (strcmp(s, "~")==0 || strcmp(s, "+")==0 || strcmp(s, "*")==0 || strcmp(s, ">")==0);
}

static Node* build() {
    if (idx >= toks_n) return NULL;
    char *tk = toks[idx++];
    Node *n = newNode(tk);
    if (strcmp(tk, "~") == 0) {
        n->right = build();
    } else if (isOpTok(tk)) {
        n->left = build();
        n->right = build();
    } else {
        // leaf
    }
    return n;
}

Node* prefixToParseTree(const char *prefix) {
    idx = 0;
    toks = tokenize(prefix, &toks_n);
    Node *root = build();
    for (int i=0;i<toks_n;i++) free(toks[i]);
    free(toks);
    return root;
}
