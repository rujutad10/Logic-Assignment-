
// Define a large buffer size to prevent overflows with CNF files.
// This matches the size used in cnfReader.c
#define LARGE_BUFFER_SIZE 2000000
/**
 * @file mainfhc.c
 * @brief Main driver program for the Logic in CS Assignment.
 *
 * This program allows the user to either input a formula manually in infix notation
 * or process a DIMACS .cnf file. It then performs several tasks:
 *   - Task 1: Convert infix to prefix
 *   - Task 2: Build a parse tree from the prefix
 *   - Task 3: In-order traversal
 *   - Task 4: Compute parse tree height
 *   - Task 5: Generate truth table and evaluate
 *   - Task 6: Convert to CNF (for manual input)
 *   - Task 7: Check CNF validity
 *
 * Large buffers are dynamically allocated to handle large CNF files.
 */

#include "task3.h"
#include "task4.h"
#include "task5.h"
#include "task6.h"
#include "task7.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "cnfReader.h"
#include "task1.h"
#include "task2.h"

#define LARGE_BUFFER_SIZE 2000000

/**
 * @brief Entry point for the program.
 *
 * Handles user input, processing tasks 1-7, timing, and cleanup.
 *
 * @return int 0 on successful completion, 1 on error.
 */
int main()
{
    // --- Use malloc for large, dynamically allocated buffers ---
    char *inputInfix = malloc(LARGE_BUFFER_SIZE);
    char *inputPrefix = malloc(LARGE_BUFFER_SIZE);
    
    // Always check if malloc was successful
    if (inputInfix == NULL || inputPrefix == NULL) {
        printf("Fatal Error: Failed to allocate memory for buffers.\n");
        return 1;
    }

    Node *Root = NULL;
    Node *cnRoot = NULL;
    
    int choice;
    char filename[256];

    clock_t start, end;
    double total_time_taken;

    // --- 1. User Input Menu ---
    printf("========================================\n");
    printf(" CS F214 - Logic in CS Assignment\n");
    printf("========================================\n");
    printf("  1. Enter Infix Formula Manually\n");
    printf("  2. Process a .cnf file\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        free(inputInfix);
        free(inputPrefix);
        return 1;
    }

    // --- 2. Get the Formula ---
    if (choice == 1)
    {
        printf("\n(Please use single uppercase letters as atoms: A, B, C...)\n");
        printf("Enter Infix Notation: ");
        scanf("%s", inputInfix);
    }
    else if (choice == 2)
    {
        printf("\nEnter .cnf file path: ");
        scanf("%s", filename);

        char *formulaFromFile = cnfToInfix(filename);
        if (formulaFromFile == NULL) {
            printf("Error: Could not read or process file '%s'.\n", filename);
            free(inputInfix);
            free(inputPrefix);
            return 1;
        }

        // Copy the formula, ensuring we don't overflow our large buffer
        strncpy(inputInfix, formulaFromFile, LARGE_BUFFER_SIZE - 1);
        inputInfix[LARGE_BUFFER_SIZE - 1] = '\0';
        
        free(formulaFromFile); // Free the temporary buffer from cnfReader

        printf("\nSuccessfully loaded formula from %s\n", filename);
    }
    else
    {
        printf("Invalid choice. Exiting.\n");
        free(inputInfix);
        free(inputPrefix);
        return 1;
    }

    // --- 3. Start Processing ---
    printf("\n--- Starting All Tasks ---\n");
    start = clock();

    // --- Task 1: Infix to Prefix ---
    printf("\n[Task 1] Converting Infix to Prefix...\n");
    inFixToPreFix(inputInfix, inputPrefix);

    // --- Task 2: Prefix to Parse Tree ---
    printf("\n[Task 2] Building Parse Tree from Prefix...\n");
    convertPreOrderToTree(&Root, inputPrefix);
    if (Root == NULL) {
        printf("Error: Failed to build parse tree. Check your input.\n");
        free(inputInfix);
        free(inputPrefix);
        return 1;
    }
    printf("Parse Tree built successfully.\n");

    // --- Task 3: In-order Traversal ---
    printf("\n[Task 3] Re-printing expression (In-Order Traversal)...\n");
    printf("Infix Expression: ");
    printPreOrder(Root);
    printf("\n");

    // --- Task 4: Compute Tree Height ---
    printf("\n[Task 4] Calculating Height of Parse Tree...\n");
    printf("The Height of the Parse Tree is: %d\n", maxHeightOfParseTree(Root));

    // --- Task 5: Truth Table & Evaluation ---
    printf("\n[Task 5] Generating Truth Table...\n");
    printTruthTable(Root);

    // --- Task 6: Convert to CNF (CONDITIONAL) ---
    if (choice == 1)
    {
        printf("\n[Task 6] Converting to CNF (Manual Input)...\n");
        cnRoot = convertToCNF(Root);
        printf("CNF Formula: ");
        printCNF(cnRoot);
        printf("\n");
    }
    else
    {
        printf("\n[Task 6] Skipped (Input from .cnf file is already in CNF).\n");
        cnRoot = Root;
    }

    // --- Task 7: CNF Validity Check ---
    printf("\n[Task 7] Checking CNF Validity...\n");
    int valid = 0, invalid = 0;
    checkCNFValidity(cnRoot, &valid, &invalid);
    
    printf("Valid Clauses: %d\n", valid);
    printf("Invalid Clauses: %d\n", invalid);
    if (invalid == 0 && valid > 0) {
        printf("Result: The formula is VALID (a Tautology).\n");
    } else {
        printf("Result: The formula is NOT VALID.\n");
    }

    // --- 4. Final Timing ---
    end = clock();
    total_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n----------------------------------------\n");
    printf("All tasks complete.\n");
    printf("Total execution time: %f seconds\n", total_time_taken);
    printf("----------------------------------------\n");

    // --- 5. Final Cleanup ---
    printf("Freeing memory...\n");
    freeTree(Root); // This frees the original tree
    if (choice == 1 && cnRoot != Root) {
         // If choice was 1, cnRoot is a *copy* and must also be freed
         freeTree(cnRoot);
    }
    free(inputInfix);
    free(inputPrefix);

    return 0;
}

