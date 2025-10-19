// task2.h
#ifndef TASK2_H
#define TASK2_H

#include "common.h" 
void inFixToPreFix(char *input, char *outputToChar);

// From task2.c
void convertPreOrderToTree(Node **root, char *start);
void printPreOrder(Node *root);
void printPostOrder(Node *root);

// From task3.c
void printInOrder(Node *root);

// From task4.c
int maxheightOfParseTree(Node *root);

// From task5.c
void printTruthTable(Node *root);
int truthEvaluator(Node *root); // Simplified signature

// From task6.c
Node* convertToCNF(Node* root);
void printCNF(Node* root);

// From task7.c
void checkCNFValidity(Node* root, int *validCount, int *InvalidCount);
int isClauseValid(Node* clause);

#endif
