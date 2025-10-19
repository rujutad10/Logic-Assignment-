#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h" 
#include <stdlib.h>

#define MAX_TOKENS 1024
#define MAX_TOKEN_LEN 64

// --- New Stack for Strings (char*) ---
char* stack[MAX_TOKENS];
int top = -1;

void stackPush(char* s) {
    stack[++top] = s;
}

char* stackPop() {
    if (top == -1) return NULL;
    return stack[top--];
}

int stackIsEmpty() {
    return top == -1;
}

// --- Priority Function for Strings ---
int priority(char* op) {
    if (strcmp(op, "(") == 0) return 1;
    if (strcmp(op, ">") == 0) return 2;
    if (strcmp(op, "+") == 0) return 3;
    if (strcmp(op, "*") == 0) return 4;
    if (strcmp(op, "~") == 0) return 5;
    return 0; // Atom
}

// --- Helper to Check for Atoms ---
// An atom is anything that isn't an operator or parenthesis
int isAtom(char* tok) {
    if (priority(tok) == 0 && strcmp(tok, ")") != 0) {
        return 1;
    }
    return 0;
}

// --- Tokenizer Function ---
// This function splits "(x1+~x2)" into tokens: "(", "x1", "+", "~", "x2", ")"
int tokenize(char* input, char* tokens[]) {
    int count = 0;
    char* p = input;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;

    while (*p != '\0') {
        if (isspace(*p)) {
            p++; // Skip whitespace
            continue;
        }

        // Operators are single chars
        if (*p == '(' || *p == ')' || *p == '+' || *p == '*' || *p == '~' || *p == '>') {
            buffer[0] = *p;
            buffer[1] = '\0';
            tokens[count++] = strdup_s(buffer);
            p++;
        }
        // Atoms (operands) start with 'x' (or a letter) and are followed by digits
        else if (isalpha(*p)) {
            i = 0;
            buffer[i++] = *p;
            p++;
            while (isdigit(*p)) {
                buffer[i++] = *p;
                p++;
            }
            buffer[i] = '\0';
            tokens[count++] = strdup_s(buffer);
        }
        else {
            // Unknown character
            p++;
        }
    }
    return count;
}

// --- inFixToPreFix Function ---
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

    // 3. Swap parentheses in the token array
    for (int i = 0; i < n; i++) {
        if (strcmp(tokens[i], "(") == 0) {
            tokens[i] = ")";
        } else if (strcmp(tokens[i], ")") == 0) {
            tokens[i] = "(";
        }
    }

    // 4. Infix-to-Postfix algorithm on the reversed tokens
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
            stackPop(); // Pop '('
        } else { // It's an operator
            while (!stackIsEmpty() && strcmp(stack[top], "(") != 0 && priority(stack[top]) >= priority(tok)) {
                output[outputCounter++] = stackPop();
            }
            stackPush(tok);
        }
    }

    // Pop remaining operators
    while (!stackIsEmpty()) {
        output[outputCounter++] = stackPop();
    }

    // 5. Reverse output tokens and build final prefix string
    outputToChar[0] = '\0'; // Start with an empty string
    printf("Output in PreOrder (Prefix) Notation: ");
    for (int i = outputCounter - 1; i >= 0; i--) {
        printf("%s ", output[i]);
        // Concatenate into the final output string with spaces
        strcat(outputToChar, output[i]);
        strcat(outputToChar, " ");
    }
    printf("\n");

    // Free allocated tokens (important!)
    for (int i = 0; i < n; i++) {
        free(tokens[i]);
    }
}
