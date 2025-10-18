#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*typedef struct TreeNode {
    char val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;*/

/*TreeNode* createNode(char val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = node->right = NULL;
    return node;
}*/

// Check if a clause is valid (contains a literal and its negation)
int isClauseValid(TreeNode* clause) {
    if (!clause) return 0;

    int pos[26] = {0}; // positive literals A-Z
    int neg[26] = {0}; // negated literals ~A-~Z

    //Search to collect literals in this clause
    TreeNode* stack[100];
    int top = -1;
    stack[++top] = clause;

    while (top >= 0) {
        TreeNode* node = stack[top--];
        if (!node) continue;

        if (isalpha(node->val)) pos[node->val - 'A'] = 1;
        else if (node->val == '~' && node->left && isalpha(node->left->val))
            neg[node->left->val - 'A'] = 1;
        else {
            stack[++top] = node->left;
            stack[++top] = node->right;
        }
    }

    // check if any literal and its negation exist
    for (int i = 0; i < 26; i++) {
        if (pos[i] && neg[i]) return 1; // valid clause
    }

    return 0; // invalid clause
}

// Traverse CNF tree and count valid/invalid clauses
void checkCNFValidity(TreeNode* root, int *validCount, int *invalidCount) {
    if (!root) return;

    if (root->val == '*') { // conjunction: go to left and right
        checkCNFValidity(root->left, validCount, invalidCount);
        checkCNFValidity(root->right, validCount, invalidCount);
    } else { // this is a single clause
        if (isClauseValid(root)) (*validCount)++;
        else (*invalidCount)++;
    }
}

// Print CNF tree (for testing)
/*void printCNF(TreeNode* root) {
    if (!root) return;
    if (root->val == '*' || root->val == '+') printf("(");
    printCNF(root->left);
    printf("%c", root->val);
    printCNF(root->right);
    if (root->val == '*' || root->val == '+') printf(")");
}*/

// ----------------- Main -----------------
/*int main() {
    // Example CNF: (A + ~A + B) * (B + C)
    TreeNode* clause1 = createNode('+');
    clause1->left = createNode('A');
    clause1->right = createNode('+');
    clause1->right->left = createNode('~');
    clause1->right->left->left = createNode('A');
    clause1->right->right = createNode('B');

    TreeNode* clause2 = createNode('+');
    clause2->left = createNode('B');
    clause2->right = createNode('C');

    TreeNode* cnfRoot = createNode('*');
    cnfRoot->left = clause1;
    cnfRoot->right = clause2;

    printf("CNF Formula: ");
    printCNF(cnfRoot);
    printf("\n");

    int valid = 0, invalid = 0;
    checkCNFValidity(cnfRoot, &valid, &invalid);

    printf("Valid clauses: %d\n", valid);
    printf("Invalid clauses: %d\n", invalid);

    return 0;
}*/
