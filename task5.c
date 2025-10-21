/**
 * @file task5.c
 * @brief Implements truth table generation for logical formulas.
 *
 * This module evaluates all possible truth assignments for variables
 * in a logical formula and prints (and optionally saves) the complete
 * truth table.
 * @section algo Algorithm: Exhaustive enumeration + formula evaluation
 *   Generate 2^n assignments, evaluate formula for each
 * @section time Time Complexity: O(n × 2^n)
 *   - 2^n possible assignments
 *   - Evaluate formula: O(n) per assignment
 *   - Overall: exponential growth
 *   - PRACTICAL LIMIT: ~15-20 variables max
 * @section space Space Complexity: O(n)
 *   - Variable array: O(n)
 *   - Assignment array: O(n)
 *   - Output: O(n × 2^n) if saved to file
 *   - Stack: O(h) for evaluation recursion
 * @note Task skipped if variables > 10 to prevent timeout
 */
#include "task2.h"  // for printInOrder
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @struct TruthAssignment
 * @brief Represents a mapping of a variable name to its truth value.
 */
typedef struct {
    char* variable;  /**< Variable name (e.g., "A" or "x1") */
    int value;       /**< Truth value (0 for False, 1 for True) */
} TruthAssignment;

// --- Function Prototypes ---
void collectVariables(Node* root, char* vars[], int* varCount);
int evaluateFormula(Node* root, TruthAssignment assignments[], int assignmentCount);
void printAndSaveTable(Node* root, char* vars[], int varCount, FILE* file);


/**
 * @brief Main entry point for truth table generation.
 *
 * Collects all unique variables, computes truth values for all possible
 * combinations, and prints or saves the table.
 *
 * @param root Root node of the parse tree.
 */
void printTruthTable(Node *root) {
    if (!root) {
        printf("Error: Cannot generate truth table for an empty formula.\n");
        return;
    }

    char* variables[2048];
    int varCount = 0;
    collectVariables(root, variables, &varCount);

    if (varCount == 0) {
        printf("No variables found in formula.\n");
        TruthAssignment assignments[1];
        int result = evaluateFormula(root, assignments, 0);
        printf("\nResult of constant formula: %s\n", result ? "T" : "F");
        return;
    }

    // Warn user if table is large
    if (varCount > 10) {
        long long numRows = 1LL << varCount;
        printf("Warning: %d variables will generate %lld rows.\n", varCount, numRows);
        printf("Proceed? (y/n): ");
        char choice = 'n';
        while (getchar() != '\n');
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            for (int i = 0; i < varCount; i++) free(variables[i]);
            return;
        }
    }

    printAndSaveTable(root, variables, varCount, NULL);

    printf("\nDo you want to save the truth table to a file? (y/n): ");
    char saveChoice = 'n';
    while (getchar() != '\n');
    scanf(" %c", &saveChoice);

    if (saveChoice == 'y' || saveChoice == 'Y') {
        char filename[256];
        printf("Enter filename: ");
        scanf("%255s", filename);
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error: Cannot open file '%s'\n", filename);
        } else {
            printAndSaveTable(root, variables, varCount, file);
            fclose(file);
            printf("Truth table saved to '%s'\n", filename);
        }
    }

    for (int i = 0; i < varCount; i++) free(variables[i]);
}

/**
 * @brief Recursively collects all unique variable names from the parse tree.
 *
 * @param root Current node of the tree.
 * @param vars Output array of variable strings.
 * @param varCount Pointer to the count of unique variables.
 */
void collectVariables(Node* root, char* vars[], int* varCount) {
    if (!root) return;

    if (root->left == NULL && root->right == NULL) {
        for (int i = 0; i < *varCount; i++)
            if (strcmp(vars[i], root->tok) == 0) return;

        vars[*varCount] = strdup_s(root->tok);
        (*varCount)++;
    }

    collectVariables(root->left, vars, varCount);
    collectVariables(root->right, vars, varCount);
}

/**
 * @brief Evaluates the truth value of the given formula.
 *
 * Supports operators:  
 * - `~` (NOT),  
 * - `+` (OR),  
 * - `*` (AND),  
 * - `>` (IMPLIES)
 *
 * @param root Root of the parse tree.
 * @param assignments Array of variable-value mappings.
 * @param assignmentCount Number of assignments.
 * @return 1 for True, 0 for False.
 */
int evaluateFormula(Node* root, TruthAssignment assignments[], int assignmentCount) {
    if (root->left == NULL && root->right == NULL) {
        for (int i = 0; i < assignmentCount; i++)
            if (strcmp(root->tok, assignments[i].variable) == 0)
                return assignments[i].value;
        return -1;
    }

    if (strcmp(root->tok, "~") == 0)
        return !evaluateFormula(root->right, assignments, assignmentCount);

    int leftVal = evaluateFormula(root->left, assignments, assignmentCount);
    int rightVal = evaluateFormula(root->right, assignments, assignmentCount);

    if (strcmp(root->tok, "+") == 0) return leftVal || rightVal;
    if (strcmp(root->tok, "*") == 0) return leftVal && rightVal;
    if (strcmp(root->tok, ">") == 0) return !leftVal || rightVal;

    return -1;
}

/**
 * @brief Prints the truth table to console or file.
 *
 * @param root Root of the formula tree.
 * @param vars Array of variable names.
 * @param varCount Count of variables.
 * @param file File pointer (NULL for console output).
 */
void printAndSaveTable(Node* root, char* vars[], int varCount, FILE* file) {
    #define PRINTF_TARGET(fmt, ...) (file ? fprintf(file, fmt, __VA_ARGS__) : printf(fmt, __VA_ARGS__))
    #define PRINTF_SIMPLE(txt) (file ? fprintf(file, "%s", txt) : printf("%s", txt))

    if (file == NULL) {
        printf("\nTruth Table for: ");
        printInOrder(root);
        printf("\n\n");
    }

    for (int i = 0; i < varCount; i++) PRINTF_TARGET("%s\t", vars[i]);
    PRINTF_SIMPLE(" | Result\n");

    long long total = 1LL << varCount;
    for (long long i = 0; i < total; i++) {
        TruthAssignment current[varCount];
        for (int j = 0; j < varCount; j++) {
            current[j].variable = vars[j];
            current[j].value = (i >> (varCount - 1 - j)) & 1;
            PRINTF_TARGET("%d\t", current[j].value);
        }
        int result = evaluateFormula(root, current, varCount);
        PRINTF_TARGET(" | %s\n", result ? "T" : "F");
    }
}
