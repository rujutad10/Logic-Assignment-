#include "common.h"
#include <stdio.h>
#include <ctype.h>   
#include <stdlib.h>  
#include <string.h>
#include <math.h>

struct AtomValue {
    char name[64];
    int value;
} truthValues[1024]; // Can hold up to 1024 unique atoms
int globalAtomCount = 0;

//evaluating the logical expression recursively(bottom-up)
// New helper to get the value for an atom
int returnValueForAtom(char* tok) {
    for (int i = 0; i < globalAtomCount; i++) {
        if (strcmp(truthValues[i].name, tok) == 0) {
            return truthValues[i].value;
        }
    }
    return -1; // Error
}

int truthEvaluator(Node *root) {
    // Base case: It's an atom
    if (root->left == NULL && root->right == NULL) {
        return returnValueForAtom(root->tok);
    }

    // Recursive cases
    if (strcmp(root->tok, "~") == 0) {
        return !(truthEvaluator(root->right));
    }

    // Binary operators
    int leftVal = truthEvaluator(root->left);
    int rightVal = truthEvaluator(root->right);

    if (strcmp(root->tok, "+") == 0) {
        return leftVal | rightVal;
    }
    if (strcmp(root->tok, "*") == 0) {
        return leftVal & rightVal;
    }
    if (strcmp(root->tok, ">") == 0) {
        return (!leftVal) | rightVal;
    }

    return -1; // Error
}
// Helper to check if an atom is already in our list
int isAtomInList(char* tok) {
    for (int i = 0; i < globalAtomCount; i++) {
        if (strcmp(truthValues[i].name, tok) == 0) return 1;
    }
    return 0;
}

void collectAtoms(Node *root) {
    if (root == NULL) return;

    // Check if it's an atom
    if (root->left == NULL && root->right == NULL) {
        if (!isAtomInList(root->tok)) {
            strcpy(truthValues[globalAtomCount].name, root->tok);
            globalAtomCount++;
        }
    }
    collectAtoms(root->left);
    collectAtoms(root->right);
}
// Function to print the complete truth table
void printTruthTable(Node *root) {
    globalAtomCount = 0;
    collectAtoms(root);

    if (globalAtomCount == 0) {
        printf("No propositional atoms found.\n");
        return;
    }

    int rows = pow(2, globalAtomCount);
    printf("\nTruth Table:\n");
    for (int i = 0; i < globalAtomCount; i++) printf("%s ", truthValues[i].name);
    printf("| Result\n");

    // ... (print header line) ...

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < globalAtomCount; j++) {
            int val = (i >> (globalAtomCount - j - 1)) & 1;
            truthValues[j].value = val;
            printf("%d  ", val);
        }
        int result = truthEvaluator(root); // New call
        printf("|   %d\n", result);
    }
}

