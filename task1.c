#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h" // Use the common header for Node and strdup_s

#define MAX_TOKENS 8192
#define MAX_TOKEN_LEN 64

// --- Stack for storing string tokens (char*) ---
char* stack[MAX_TOKENS];
int top = -1;

void stackPush(char* s) {
    if (top < MAX_TOKENS - 1) {
        stack[++top] = s;
    }
}

char* stackPop() {
    if (top == -1) return NULL;
    return stack[top--];
}

int stackIsEmpty() {
    return top == -1;
}

// --- Priority function for operators (as strings) ---
static int priority(const char* op) {
    if (strcmp(op, "(") == 0) return 1;
    if (strcmp(op, ">") == 0) return 2;
    if (strcmp(op, "+") == 0) return 3;
    if (strcmp(op, "*") == 0) return 4;
    if (strcmp(op, "~") == 0) return 5;
    return 0; // Atom
}

// --- Helper to check if a token is an atom ---
static int isAtom(const char* tok) {
    // An atom is any token that is not a known operator or a parenthesis.
    return (priority(tok) == 0 && strcmp(tok, ")") != 0 && strcmp(tok, "(") != 0);
}

/**
 * @brief Tokenizes an infix string into an array of string tokens.
 * This is the first step of the conversion.
 * @param input The raw infix formula string.
 * @param tokens An array of char* to be filled with tokens.
 * @return The number of tokens found.
 */
static int tokenize(const char* input, char* tokens[]) {
    int count = 0;
    const char* p = input;
    char buffer[MAX_TOKEN_LEN];

    while (*p != '\0' && count < MAX_TOKENS) {
        // Skip whitespace
        while (isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') break;

        // Operators and parentheses are single-character tokens
        if (strchr("()~*+>", *p)) {
            buffer[0] = *p;
            buffer[1] = '\0';
            tokens[count++] = strdup_s(buffer);
            p++;
        }
        // Atoms (variables) start with a letter, followed by letters or digits
        else if (isalpha((unsigned char)*p)) {
            int i = 0;
            while (isalnum((unsigned char)*p) && i < MAX_TOKEN_LEN - 1) {
                buffer[i++] = *p++;
            }
            buffer[i] = '\0';
            tokens[count++] = strdup_s(buffer);
        }
        else {
            // Handle unknown characters by skipping them
            p++;
        }
    }
    return count;
}


/**
 * @brief Converts a fully-parenthesized infix formula to prefix notation.
 * This version is memory-safe and handles string tokens.
 * @param input The infix formula string.
 * @param outputToChar A large buffer to store the final, space-separated prefix string.
 */
void inFixToPreFix(char *input, char *outputToChar) {
    char* tokens[MAX_TOKENS];
    char* output[MAX_TOKENS];
    int outputCounter = 0;
    top = -1; // Reset stack

    // 1. Tokenize the input string
    int n = tokenize(input, tokens);

    // 2. Reverse the token array
    for (int i = 0; i < n / 2; i++) {
        char* temp = tokens[i];
        tokens[i] = tokens[n - 1 - i];
        tokens[n - 1 - i] = temp;
    }

    // 3. Swap parentheses (MEMORY-SAFE VERSION)
    for (int i = 0; i < n; i++) {
        if (strcmp(tokens[i], "(") == 0) {
            free(tokens[i]); // Free the old token "("
            tokens[i] = strdup_s(")"); // Allocate memory for the new token ")"
        } else if (strcmp(tokens[i], ")") == 0) {
            free(tokens[i]); // Free the old token ")"
            tokens[i] = strdup_s("("); // Allocate memory for the new token "("
        }
    }

    // 4. Standard Infix-to-Postfix algorithm on the reversed tokens
    for (int i = 0; i < n; i++) {
        char* tok = tokens[i];

        if (isAtom(tok)) {
            output[outputCounter++] = tok;
        } else if (strcmp(tok, "(") == 0) {
            stackPush(tok);
        } else if (strcmp(tok, ")") == 0) {
            while (!stackIsEmpty() && strcmp(stack[top], "(") != 0) {
                output[outputCounter++] = stackPop();
            }
            if (!stackIsEmpty()) {
                stackPop(); // Pop and discard the '('
            }
        } else { // It's an operator
            while (!stackIsEmpty() && strcmp(stack[top], "(") != 0 && priority(stack[top]) >= priority(tok)) {
                output[outputCounter++] = stackPop();
            }
            stackPush(tok);
        }
    }

    // Pop remaining operators from the stack
    while (!stackIsEmpty()) {
        output[outputCounter++] = stackPop();
    }

    // 5. Reverse the output tokens and build the final prefix string
    outputToChar[0] = '\0'; // Start with an empty string
    for (int i = outputCounter - 1; i >= 0; i--) {
        strcat(outputToChar, output[i]);
        strcat(outputToChar, " "); // Add space between tokens
    }

    // 6. Final, CRITICAL cleanup: free all allocated tokens
    for (int i = 0; i < n; i++) {
        free(tokens[i]);
    }
}
