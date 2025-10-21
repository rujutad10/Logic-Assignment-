/**
 * @file task1.c
 * @brief Converts fully-parenthesized infix logical expressions into prefix notation.
 *
 * Uses tokenization, operator precedence, and stack-based processing to
 * safely transform infix expressions into prefix form.
 * * @section time Time Complexity: O(n)
 *   - Tokenization: O(n)
 *   - Reversal: O(n)
 *   - Shunting Yard: O(n)
 * @section space Space Complexity: O(n)
 *   - Stack: O(n), Output: O(n), Tokens: O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

#define MAX_TOKENS 8192
#define MAX_TOKEN_LEN 64

// --- Stack for string tokens ---
static char* stack[MAX_TOKENS];
static int top = -1;

/**
 * @brief Pushes a token onto the stack.
 * @param s The string token to push.
 */
static void stackPush(char* s) {
    if (top < MAX_TOKENS - 1) {
        stack[++top] = s;
    }
}

/**
 * @brief Pops a token from the stack.
 * @return Pointer to the popped string token, or NULL if empty.
 */
static char* stackPop() {
    if (top == -1) return NULL;
    return stack[top--];
}

/**
 * @brief Checks if the stack is empty.
 * @return 1 if empty, 0 otherwise.
 */
static int stackIsEmpty() {
    return top == -1;
}

/**
 * @brief Returns operator precedence value.
 * @param op Operator token as string.
 * @return Integer priority (higher = tighter binding).
 */
static int priority(const char* op) {
    if (strcmp(op, "(") == 0) return 1;
    if (strcmp(op, ">") == 0) return 2;
    if (strcmp(op, "+") == 0) return 3;
    if (strcmp(op, "*") == 0) return 4;
    if (strcmp(op, "~") == 0) return 5;
    return 0;
}

/**
 * @brief Checks if a token is an atom (variable).
 * @param tok Token string.
 * @return 1 if atom, 0 otherwise.
 */
static int isAtom(const char* tok) {
    return (priority(tok) == 0 && strcmp(tok, ")") != 0 && strcmp(tok, "(") != 0);
}

/**
 * @brief Tokenizes the infix string into an array of tokens.
 * @param input Input infix expression string.
 * @param tokens Output array of string pointers.
 * @return Number of tokens parsed.
 */
static int tokenize(const char* input, char* tokens[]) {
    int count = 0;
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];

    while (*p != '\0' && count < MAX_TOKENS) {
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') break;

        if (strchr("()~*+>", *p)) {
            buffer[0] = *p;
            buffer[1] = '\0';
            tokens[count++] = strdup_s(buffer);
            p++;
        } else if (isalpha((unsigned char)*p)) {
            int i = 0;
            while (isalnum((unsigned char)*p) && i < MAX_TOKEN_LEN - 1)
                buffer[i++] = *p++;
            buffer[i] = '\0';
            tokens[count++] = strdup_s(buffer);
        } else {
            p++;
        }
    }
    return count;
}

/**
 * @brief Converts infix to prefix (core function).
 * @param input Infix expression.
 * @param outputToChar Output buffer for prefix expression.
 */
void inFixToPreFix(char *input, char *outputToChar) {
    char* tokens[MAX_TOKENS];
    char* output[MAX_TOKENS];
    int outputCounter = 0;
    top = -1;

    int n = tokenize(input, tokens);

    // Reverse tokens
    for (int i = 0; i < n / 2; i++) {
        char* temp = tokens[i];
        tokens[i] = tokens[n - 1 - i];
        tokens[n - 1 - i] = temp;
    }

    // Swap parentheses safely
    for (int i = 0; i < n; i++) {
        if (strcmp(tokens[i], "(") == 0) {
            free(tokens[i]);
            tokens[i] = strdup_s(")");
        } else if (strcmp(tokens[i], ")") == 0) {
            free(tokens[i]);
            tokens[i] = strdup_s("(");
        }
    }

    // Convert to postfix on reversed tokens
    for (int i = 0; i < n; i++) {
        char* tok = tokens[i];

        if (isAtom(tok)) {
            output[outputCounter++] = tok;
        } else if (strcmp(tok, "(") == 0) {
            stackPush(tok);
        } else if (strcmp(tok, ")") == 0) {
            while (!stackIsEmpty() && strcmp(stack[top], "(") != 0)
                output[outputCounter++] = stackPop();
            if (!stackIsEmpty()) stackPop();
        } else {
            while (!stackIsEmpty() && strcmp(stack[top], "(") != 0 &&
                   priority(stack[top]) >= priority(tok))
                output[outputCounter++] = stackPop();
            stackPush(tok);
        }
    }

    while (!stackIsEmpty()) {
        output[outputCounter++] = stackPop();
    }

    // Build prefix string
    outputToChar[0] = '\0';
    for (int i = outputCounter - 1; i >= 0; i--) {
        strcat(outputToChar, output[i]);
        strcat(outputToChar, " ");
    }

    // Cleanup
    for (int i = 0; i < n; i++) free(tokens[i]);
}
