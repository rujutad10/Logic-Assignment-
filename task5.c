#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// --- Data Structure to hold a truth assignment (e.g., A = 1, B = 0) ---
// Adapted to use strings (char*) for variable names like "x1", "A", etc.
typedef struct {
    char* variable;
    int value;
} TruthAssignment;

// --- Function Prototypes for this file ---
void collectVariables(Node* root, char* vars[], int* varCount);
int evaluateFormula(Node* root, TruthAssignment assignments[], int assignmentCount);
void printAndSaveTable(Node* root, char* vars[], int varCount, FILE* file);
void printInOrder(Node* root); // We need this prototype here


/**
 * @brief Main function for Task 5. Orchestrates the truth table generation.
 * @param root The root of the parse tree.
 */
void printTruthTable(Node *root) {
    if (!root) {
        printf("Error: Cannot generate truth table for an empty formula.\n");
        return;
    }

    char* variables[2048]; // Array to hold pointers to variable name strings
    int varCount = 0;
    collectVariables(root, variables, &varCount);

    if (varCount == 0) {
        printf("No variables found in formula.\n");
        // We can still evaluate a constant formula
        TruthAssignment assignments[1];
        int result = evaluateFormula(root, assignments, 0);
        printf("\nResult of constant formula: %s\n", result ? "T" : "F");
        return;
    }

    // --- User-Friendly Warning for Large Tables (from the snippet) ---
    if (varCount > 10) {
        long long numRows = 1LL << varCount; // Use long long for large numbers
        printf("Warning: %d variables will generate %lld rows. This may be very slow.\n", varCount, numRows);
        printf("Proceed? (y/n): ");
        char choice = 'n';
        // Clear input buffer before reading char
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            printf("Aborting truth table generation.\n");
            // Free the memory allocated by strdup_s in collectVariables
            for (int i = 0; i < varCount; i++) {
                free(variables[i]);
            }
            return;
        }
    }

    // Print the table to the console
    printAndSaveTable(root, variables, varCount, NULL);

    // --- Save-to-File Logic (from the snippet) ---
    printf("\nDo you want to save the truth table to a file? (y/n): ");
    char saveChoice = 'n';
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    scanf(" %c", &saveChoice);

    if (saveChoice == 'y' || saveChoice == 'Y') {
        char filename[256];
        printf("Enter filename: ");
        scanf("%255s", filename);
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error: Cannot open file '%s' for writing.\n", filename);
        } else {
            // Print the same table, but this time directing output to the file
            printAndSaveTable(root, variables, varCount, file);
            fclose(file);
            printf("Truth table successfully saved to '%s'\n", filename);
        }
    }

    // Free the memory allocated for variable names
    for (int i = 0; i < varCount; i++) {
        free(variables[i]);
    }
}


/**
 * @brief Traverses the tree to find all unique variable names.
 * @param root The current node in the tree.
 * @param vars An array to store the unique variable strings.
 * @param varCount A pointer to the count of unique variables found.
 */
void collectVariables(Node* root, char* vars[], int* varCount) {
    if (!root) return;

    // A variable is a leaf node (it has no children)
    if (root->left == NULL && root->right == NULL) {
        // Check if we've already seen this variable
        for (int i = 0; i < *varCount; i++) {
            if (strcmp(vars[i], root->tok) == 0) {
                return; // Already in the list, do nothing
            }
        }
        // If not found, add a copy of it to our list
        vars[*varCount] = strdup_s(root->tok);
        (*varCount)++;
    }

    collectVariables(root->left, vars, varCount);
    collectVariables(root->right, vars, varCount);
}

/**
 * @brief Evaluates the truth value of a formula for a given set of assignments.
 * @param root The root of the formula's parse tree.
 * @param assignments An array of variable names and their current truth values.
 * @param assignmentCount The number of variables in the assignment.
 * @return 1 for True, 0 for False.
 */
int evaluateFormula(Node* root, TruthAssignment assignments[], int assignmentCount) {
    // Base case: If the node is a variable, find its value in the assignments
    if (root->left == NULL && root->right == NULL) {
        for (int i = 0; i < assignmentCount; i++) {
            if (strcmp(root->tok, assignments[i].variable) == 0) {
                return assignments[i].value;
            }
        }
        return -1; // Should not happen if all variables are found
    }

    // Recursive case for operators
    if (strcmp(root->tok, "~") == 0) {
        return !evaluateFormula(root->right, assignments, assignmentCount);
    }

    int leftVal = evaluateFormula(root->left, assignments, assignmentCount);
    int rightVal = evaluateFormula(root->right, assignments, assignmentCount);

    if (strcmp(root->tok, "+") == 0) return leftVal || rightVal;
    if (strcmp(root->tok, "*") == 0) return leftVal && rightVal;
    if (strcmp(root->tok, ">") == 0) return !leftVal || rightVal;

    return -1; // Error case
}

/**
 * @brief Helper function to print the truth table to console OR a file.
 * @param root The root of the parse tree.
 * @param vars The array of unique variable names.
 * @param varCount The count of unique variables.
 * @param file A FILE pointer. If NULL, prints to console. If not NULL, prints to the file.
 */
void printAndSaveTable(Node* root, char* vars[], int varCount, FILE* file) {
    // Determine where to print: console or file
    #define PRINTF_TARGET(format, ...) (file ? fprintf(file, format, __VA_ARGS__) : printf(format, __VA_ARGS__))
    #define PRINTF_TARGET_NO_ARGS(format) (file ? fprintf(file, "%s", format) : printf("%s", format))


    // --- Print Header ---
    if (file == NULL) { // Only print formula for console output
         printf("\nTruth Table for: ");
         printInOrder(root); 
         printf("\n\n");
    }

    for (int i = 0; i < varCount; i++) {
        PRINTF_TARGET("%s\t", vars[i]);
    }
    PRINTF_TARGET_NO_ARGS(" | Result\n");
    for (int i = 0; i < varCount; i++) {
        PRINTF_TARGET_NO_ARGS("-----\t");
    }
    PRINTF_TARGET_NO_ARGS("----------\n");

    // --- Print Rows ---
    long long totalAssignments = 1LL << varCount;
    for (long long i = 0; i < totalAssignments; i++) {
        TruthAssignment currentAssignments[varCount];

        // Create the assignments for this row
        for (int j = 0; j < varCount; j++) {
            currentAssignments[j].variable = vars[j];
            // Use bitwise logic to get the correct T/F value
            currentAssignments[j].value = (i >> (varCount - 1 - j)) & 1;
            PRINTF_TARGET("%d\t", currentAssignments[j].value);
        }

        int result = evaluateFormula(root, currentAssignments, varCount);
        PRINTF_TARGET(" | %s\n", result ? "  T" : "  F");
    }
}

