#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Tree node structure
typedef struct TreeNode {
    char val;  // Operator: '~', '*', '+', '>', '<' ; or variable: 'A'-'Z'
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Utility function to create a new tree node
TreeNode* createNode(char val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = node->right = NULL;
    return node;
}

// Function to copy a tree
TreeNode* copyTree(TreeNode* root) {
    if (!root) return NULL;
    TreeNode* newNode = createNode(root->val);
    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);
    return newNode;
}

// Step 1: Eliminate implications(IMPL_FREE)
TreeNode* eliminateImplications(TreeNode* root) {
    if (!root) return NULL;
    root->left = eliminateImplications(root->left);
    root->right = eliminateImplications(root->right);

    if (root->val == '>') { // A -> B = ~A + B
        root->val = '+';
        TreeNode* notLeft = createNode('~');
        notLeft->left = root->left;
        root->left = notLeft;
    }

    return root;
}

// Step 2: Move NOT inwards(NNF)
TreeNode* moveNotInwards(TreeNode* root) {
    if (!root) return NULL;

    if (root->val == '~') {
        TreeNode* child = root->left;
        if (child->val == '~') { // ~~A = A
            TreeNode* newRoot = moveNotInwards(child->left);
            free(root);
            return newRoot;
        }
        else if (child->val == '*') { // ~(A*B) = ~A + ~B
            root->val = '+';
            TreeNode* leftNot = createNode('~'); leftNot->left = child->left;
            TreeNode* rightNot = createNode('~'); rightNot->left = child->right;
            root->left = moveNotInwards(leftNot);
            root->right = moveNotInwards(rightNot);
        }
        else if (child->val == '+') { // ~(A+B) = ~A * ~B
            root->val = '*';
            TreeNode* leftNot = createNode('~'); leftNot->left = child->left;
            TreeNode* rightNot = createNode('~'); rightNot->left = child->right;
            root->left = moveNotInwards(leftNot);
            root->right = moveNotInwards(rightNot);
        }
        else {
            root->left = moveNotInwards(root->left);
        }
    } else {
        root->left = moveNotInwards(root->left);
        root->right = moveNotInwards(root->right);
    }

    return root;
}

// Step 3: Distribute OR over AND
TreeNode* distributeOr(TreeNode* root) {
    if (!root) return NULL;
    root->left = distributeOr(root->left);
    root->right = distributeOr(root->right);

    if (root->val == '+') {
        // A + (B * C) = (A+B)*(A+C)
        if (root->right && root->right->val == '*') {
            TreeNode* newRoot = createNode('*');
            TreeNode* leftOr = createNode('+');
            leftOr->left = copyTree(root->left);
            leftOr->right = root->right->left;

            TreeNode* rightOr = createNode('+');
            rightOr->left = copyTree(root->left);
            rightOr->right = root->right->right;

            newRoot->left = distributeOr(leftOr);
            newRoot->right = distributeOr(rightOr);

            free(root);
            return newRoot;
        }
        // (A*B) + C = (A+C)*(B+C)
        if (root->left && root->left->val == '*') {
            TreeNode* newRoot = createNode('*');
            TreeNode* leftOr = createNode('+');
            leftOr->left = root->left->left;
            leftOr->right = copyTree(root->right);

            TreeNode* rightOr = createNode('+');
            rightOr->left = root->left->right;
            rightOr->right = copyTree(root->right);

            newRoot->left = distributeOr(leftOr);
            newRoot->right = distributeOr(rightOr);

            free(root);
            return newRoot;
        }
    }

    return root;
}

// Utility: Print CNF (in-order traversal with parentheses)
void printCNF(TreeNode* root) {
    if (!root) return;
    if (root->val == '*' || root->val == '+') printf("(");
    printCNF(root->left);
    printf("%c", root->val);
    printCNF(root->right);
    if (root->val == '*' || root->val == '+') printf(")");
}

int main() {
    // Example: ~(A -> B) * C
    TreeNode* root = createNode('*');
    TreeNode* left = createNode('~');
    TreeNode* imp = createNode('>');
    imp->left = createNode('A');
    imp->right = createNode('B');
    left->left = imp;
    root->left = left;
    root->right = createNode('C');

    root = eliminateImplications(root);
    root = moveNotInwards(root);
    root = distributeOr(root);

    printf("CNF: ");
    printCNF(root);
    printf("\n");

    return 0;
}
