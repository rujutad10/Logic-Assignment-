/**
 * @file main.c
 * @brief Main driver program for the Logic in CS Assignment.
 * * This program integrates all tasks. It provides a menu to either
 * enter a propositional logic formula manually or to load one
 * from a .cnf file (as per the dataset requirement).
 * * It then executes the required tasks:
 * 1. Infix to Prefix conversion.
 * 2. Prefix to Parse Tree construction.
 * 3. In-order traversal (printing the infix formula back).
 * 4. Height calculation of the parse tree.
 * 5. Truth Table generation.
 * 6. CNF Conversion (ONLY for manually entered formulas).
 * 7. CNF Validity Check (for all formulas).
 */

#include "task1.h"     // For inFixToPreFix
#include "task2.h"     // For TreeNode, convertPreOrderToTree, and all other task prototypes
#include "cnfReader.h" // For cnfToInfix

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> // For timing
#include <math.h>   // For pow() in task 5

// Define max string size
#define max 2048 // Increased size for safety with file inputs

int main()
{
    // --- Variable Declarations ---
    char inputInfix[max];
    char inputPrefix[max];
    TreeNode *Root = NULL;
    TreeNode *cnRoot = NULL; // This will hold the CNF tree
    
    int choice;
    char filename[256]; // For .cnf file path

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
        return 1;
    }

    // --- 2. Get the Formula ---
    if (choice == 1)
    {
        // --- Manual Input ---
        printf("\n(Please use single uppercase letters as atoms: A, B, C...)\n");
        printf("Enter Infix Notation: ");
        scanf("%s", inputInfix);
    }
    else if (choice == 2)
    {
        // --- File Input ---
        printf("\nEnter .cnf file path: ");
        scanf("%s", filename);

        // Call your cnfReader function
        char *formulaFromFile = cnfToInfix(filename);

        if (formulaFromFile == NULL) {
            printf("Error: Could not read or process file '%s'.\n", filename);
            return 1; // Exit on error
        }

        // Copy the formula from the buffer to our input string
        strncpy(inputInfix, formulaFromFile, max - 1);
        inputInfix[max - 1] = '\0'; // Ensure null termination
        
        // Free the memory allocated by cnfToInfix
        free(formulaFromFile);

        printf("\nSuccessfully loaded formula from %s\n", filename);
        // Optional: Print the long infix string (might be very long!)
        // printf("Formula: %s\n", inputInfix);
    }
    else
    {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

    // --- 3. Start Processing ---
    printf("\n--- Starting All Tasks ---\n");
    start = clock(); // Start the timer

    // --- Task 1: Infix to Prefix ---
    printf("\n[Task 1] Converting Infix to Prefix...\n");
    inFixToPreFix(inputInfix, inputPrefix);
    printf("Prefix Expression: %s\n", inputPrefix);

    // --- Task 2: Prefix to Parse Tree ---
    printf("\n[Task 2] Building Parse Tree from Prefix...\n");
    convertPreOrderToTree(&Root, inputPrefix);
    if (Root == NULL) {
        printf("Error: Failed to build parse tree. Check your input.\n");
        return 1;
    }
    printf("Parse Tree built successfully.\n");

    // --- Task 3: In-order Traversal ---
    printf("\n[Task 3] Re-printing expression (In-Order Traversal)...\n");
    printf("Infix Expression: ");
    printInOrder(Root);
    printf("\n");

    // --- Task 4: Compute Tree Height ---
    printf("\n[Task 4] Calculating Height of Parse Tree...\n");
    printf("The Height of the Parse Tree is: %d\n", maxheightOfParseTree(Root));

    // --- Task 5: Truth Table & Evaluation ---
    printf("\n[Task 5] Generating Truth Table...\n");
    printTruthTable(Root);

    // --- Task 6: Convert to CNF (CONDITIONAL) ---
    if (choice == 1)
    {
        // Only run CNF conversion for manually entered formulas
        printf("\n[Task 6] Converting to CNF (Manual Input)...\n");
        cnRoot = convertToCNF(Root); // Call your wrapper function
        printf("CNF Formula: ");
        printCNF(cnRoot);
        printf("\n");
    }
    else
    {
        // Input from .cnf file is ALREADY in CNF.
        printf("\n[Task 6] Skipped (Input from .cnf file is already in CNF).\n");
        // For Task 7, the CNF tree is just the original tree
        cnRoot = Root;
    }

    // --- Task 7: CNF Validity Check ---
    printf("\n[Task 7] Checking CNF Validity...\n");
    int valid = 0, invalid = 0;
    
    // Check validity on the correct tree (either converted or from file)
    checkCNFValidity(cnRoot, &valid, &invalid); 
    
    printf("Valid Clauses: %d\n", valid);
    printf("Invalid Clauses: %d\n", invalid);
    if (invalid == 0 && valid > 0) {
        printf("Result: The formula is VALID (a Tautology).\n");
    } else {
        printf("Result: The formula is NOT VALID.\n");
    }

    // --- 4. Final Timing ---
    end = clock(); // Stop the timer
    total_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n----------------------------------------\n");
    printf("All tasks complete.\n");
    printf("Total execution time: %f seconds\n", total_time_taken);
    printf("----------------------------------------\n");

    return 0;
}
