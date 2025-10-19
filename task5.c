#include "common.h"
#include <stdio.h>
#include <ctype.h>   
#include <stdlib.h>  
#include <string.h>
#include <math.h>

/*// Tree Node Structure
typedef struct TreeNode
{
    char val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;*/

int truthValues[26];

int returnValueForLetter(char c){ 
    return truthValues[toupper(c) - 'A']; 
}

//evaluating the logical expression recursively(bottom-up)
int truthEvaluator(char operation, Node *left, Node *right)
{
    // initializing left sub tree and right sub tree to 99
    int leftVal = 99, rightVal = 99;
    
    //if there is a left subtree and this isn't a unary operator(~)
    if (left != NULL && operation != '~')
    {
        // If left is not alphabet call the function recursively
        if (!isalpha(left->val))
        {
            leftVal = truthEvaluator(left->val, left->left, left->right);
        }
    }

    //same as above even for ~ as it only has right subtree
    if (right != NULL)
    {
        // If right is not alphabet call the function recursively
        if (!isalpha(right->val))
        {
            rightVal = truthEvaluator(right->val, right->left, right->right);
        }
    }

    // If Operation is ~ then there is no left root
    if (operation != '~')
    {
        if (leftVal != 0 && leftVal != 1)
        {
            leftVal = returnValueForLetter(left->val);
        }
    }

    if (rightVal != 0 && rightVal != 1)
    {
        rightVal = returnValueForLetter(right->val);
    }

    // if both are alphabets, do the operation
    switch (operation)
    {
    case '~':
        return !(rightVal);
    case '+':
        return ((leftVal) | (rightVal));
    case '*':
        return ((leftVal) & (rightVal));
    case '>':
        return ((!(leftVal)) | (rightVal));
    }

    return 0;
}

// Helper to collect all distinct atoms (A–Z) from tree
void collectAtoms(Node *root, int seen[26]) {
    if (!root) return;
    if (isalpha(root->val)) {
        seen[toupper(root->val) - 'A'] = 1;
    }
    collectAtoms(root->left, seen);
    collectAtoms(root->right, seen);
}

// Function to print the complete truth table
void printTruthTable(Node *root) {
    int seen[26] = {0};
    collectAtoms(root, seen);

    // prepare array of found atoms
    char atoms[26];
    int atomCount = 0;
    for (int i = 0; i < 26; i++)
        if (seen[i])
            atoms[atomCount++] = 'A' + i;

    if (atomCount == 0) {
        printf("No propositional atoms found.\n");
        return;
    }

    int rows = pow(2, atomCount);

    // header
    printf("\nTruth Table:\n");
    for (int i = 0; i < atomCount; i++) printf("%c ", atoms[i]);
    printf("| Result\n");
    printf("-----------------------\n");

    // iterate over all possible truth assignments
    for (int i = 0; i < rows; i++) {
        // assign truth values
        for (int j = 0; j < atomCount; j++) {
            int val = (i >> (atomCount - j - 1)) & 1;
            truthValues[atoms[j] - 'A'] = val;
            printf("%d ", val);
        }

        // evaluate the whole expression
        int result = truthEvaluator(root->val, root->left, root->right);
        printf("|   %d\n", result);
    }
}


