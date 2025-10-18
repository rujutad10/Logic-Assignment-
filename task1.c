#include "task1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Tokenize infix into array of tokens: "(", ")", "+", "*", ">", "~", "x12"
static char **tokenize(const char *s, int *n) {
    char *copy = strdup(s);
    int cap = 256;
    char **arr = malloc(sizeof(char*) * cap);
    int cnt = 0;
    int i = 0;
    int L = strlen(copy);
    while (i < L) {
        if (copy[i] == ' ' || copy[i] == '\t' || copy[i] == '\r' || copy[i] == '\n') { i++; continue; }
        if (copy[i] == '(' || copy[i] == ')' || copy[i] == '+' || copy[i] == '*' || copy[i] == '>' ) {
            char tmp[2] = { copy[i], 0 };
            arr[cnt++] = strdup(tmp);
            i++; continue;
        }
        if (copy[i] == '~') {
            arr[cnt++] = strdup("~");
            i++; continue;
        }
        if (isalnum(copy[i]) || copy[i]=='x') {
            int j = i;
            while (j < L && (isalnum(copy[j]) || copy[j]=='_' || copy[j]=='x')) j++;
            int len = j - i;
            char *tok = malloc(len + 1);
            strncpy(tok, copy + i, len);
            tok[len] = '\0';
            arr[cnt++] = tok;
            i = j;
            continue;
        }
        // unknown char skip
        i++;
        if (cnt >= cap - 10) { cap *= 2; arr = realloc(arr, sizeof(char*) * cap); }
    }
    free(copy);
    *n = cnt;
    return arr;
}

// recursive parse for fully parenthesized infix; returns prefix as string in static buffer
// uses tokens array and index pointer
static char *build_prefix(char **tokens, int *idx, int n) {
    // if token is "(" => expression of form ( A op B ) or ( ~ A )
    if (*idx >= n) return strdup("");
    char *tk = tokens[*idx];
    if (strcmp(tk, "(") == 0) {
        (*idx)++; // skip '('
        // check next token
        // parse left or unary
        char *next = tokens[*idx];
        // If next is "~" or "(" or atom, parse leftExpr
        char *leftExpr = NULL;
        if (strcmp(next, "~") == 0) {
            // unary: consume "~"
            (*idx)++;
            // parse operand
            leftExpr = build_prefix(tokens, idx, n);
            // now expect ')'
            if (strcmp(tokens[*idx], ")") == 0) { (*idx)++; }
            // prefix is "~" + operand
            char *res = malloc(strlen(leftExpr) + 3);
            sprintf(res, "~%s", leftExpr);
            free(leftExpr);
            return res;
        } else {
            // parse left
            leftExpr = build_prefix(tokens, idx, n);
            // next token should be operator
            char *op = tokens[*idx];
            (*idx)++;
            // parse right
            char *rightExpr = build_prefix(tokens, idx, n);
            // expect ')'
            if (strcmp(tokens[*idx], ")") == 0) (*idx)++;
            // construct prefix: op + left + right (no spaces)
            int L = strlen(op) + strlen(leftExpr) + strlen(rightExpr) + 3;
            char *res = malloc(L);
            res[0] = '\0';
            strcat(res, op);
            strcat(res, leftExpr);
            strcat(res, rightExpr);
            free(leftExpr);
            free(rightExpr);
            return res;
        }
    } else {
        // token is an atom like x1
        (*idx)++;
        return strdup(tk);
    }
}

void infixToPrefix(const char *infix, char *out) {
    int n;
    char **tokens = tokenize(infix, &n);
    int idx = 0;
    char *compact = build_prefix(tokens, &idx, n);
    // expand compact into spaced tokens: operators are single-char tokens ~ + * >
    char res[100000]; res[0] = '\0';
    int L = strlen(compact);
    for (int i=0;i<L;) {
        if (compact[i]=='~' || compact[i]=='+' || compact[i]=='*' || compact[i]=='>') {
            char tmp[3] = { compact[i], 0, 0 };
            strcat(res, tmp);
            strcat(res, " ");
            i++;
        } else {
            // read atom
            int j = i;
            while (j < L && compact[j] != '~' && compact[j] != '+' && compact[j] != '*' && compact[j] != '>') j++;
            strncat(res, compact + i, j - i);
            strcat(res, " ");
            i = j;
        }
    }
    // copy to out
    strcpy(out, res);
    // cleanup tokens
    for (int i=0;i<n;i++) free(tokens[i]);
    free(tokens);
    free(compact);
}
