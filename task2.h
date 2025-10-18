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


#endif
