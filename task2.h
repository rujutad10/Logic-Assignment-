// task2.h
#ifndef TASK2_H
#define TASK2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---------------------------------------------
// Tree Node Structure
// ---------------------------------------------
typedef struct TreeNode
{
    char val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// ---------------------------------------------
// Function Declarations (from task2.c)
// ---------------------------------------------
char *convertPreOrderToTree(TreeNode **root, char *start);
void printInOrder(TreeNode *root);
void printPreOrder(TreeNode *root);
void printPostOrder(TreeNode *root);

// From task3.c
void printInOrder(TreeNode *root); 

// From task4.c
int maxheightOfParseTree(TreeNode *root);
int calcMax(int a, int b); 

// From task5.c
void printTruthTable(TreeNode *root);
int truthEvaluator(char operation, TreeNode *left, TreeNode *right);
int returnValueForLetter(char c); // Needed by main.c

// From task6.c
TreeNode* createNode(char val); // Helper
TreeNode* copyTree(TreeNode* root); // Helper
TreeNode* eliminateImplications(TreeNode* root);
TreeNode* moveNotInwards(TreeNode* root);
TreeNode* distributeOr(TreeNode* root);
void printCNF(TreeNode* root);
TreeNode* convertToCNF(TreeNode* root); 

// From task7.c
void checkCNFValidity(TreeNode* root, int *validCount, int *InvalidCount);
int isClauseValid(TreeNode* clause);

#endif
