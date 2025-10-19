#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// Utility function to create a new tree node
Node* createNode(char val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->val = val;
    node->left = node->right = NULL;
    return node;
}

// Function to copy a tree
Node* copyTree(Node* root) {
    if (!root) return NULL;
    Node* newNode = createNode(root->val);
    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);
    return newNode;
}

// Step 1: Eliminate implications(IMPL_FREE)
Node* eliminateImplications(Node* root) {
    if (!root) return NULL;
    root->left = eliminateImplications(root->left);
    root->right = eliminateImplications(root->right);

    if (root->val == '>') { // A -> B = ~A + B
        root->val = '+';
        Node* notLeft = createNode('~');
        notLeft->left = root->left;
        root->left = notLeft;
    }

    return root;
}

// Step 2: Move NOT inwards(NNF)
Node* moveNotInwards(Node* root) {
    if (!root) return NULL;

    if (root->val == '~') {
        Node* child = root->left;
        if (child->val == '~') { // ~~A = A
            Node* newRoot = moveNotInwards(child->left);
            free(root);
            return newRoot;
        }
        else if (child->val == '*') { // De Morgan's: ~(A*B) = ~A + ~B
            root->val = '+'; // Change the node from ~ to +

            // new ~ node for the LEFT child
            Node* leftNot = createNode('~');
            leftNot->left = child->left; 
            // new ~ node for the RIGHT child
            Node* rightNot = createNode('~');
            rightNot->left = child->right; 

            // Recurse
            root->left = moveNotInwards(leftNot);
            root->right = moveNotInwards(rightNot);
            
            free(child); // Free the original '*' node
        }
        else if (child->val == '+') { // ~(A+B) = ~A * ~B
            root->val = '*';
            Node* leftNot = createNode('~'); leftNot->left = child->left;
            Node* rightNot = createNode('~'); rightNot->left = child->right;
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
Node* distributeOr(Node* root) {
    if (!root) return NULL;
    root->left = distributeOr(root->left);
    root->right = distributeOr(root->right);

    if (root->val == '+') {
        // A + (B * C) = (A+B)*(A+C)
        if (root->right && root->right->val == '*') {
            Node* newRoot = createNode('*');
            Node* leftOr = createNode('+');
            leftOr->left = copyTree(root->left);
            leftOr->right = root->right->left;

            Node* rightOr = createNode('+');
            rightOr->left = copyTree(root->left);
            rightOr->right = root->right->right;

            newRoot->left = distributeOr(leftOr);
            newRoot->right = distributeOr(rightOr);

            free(root);
            return newRoot;
        }
        // (A*B) + C = (A+C)*(B+C)
        if (root->left && root->left->val == '*') {
            Node* newRoot = createNode('*');
            Node* leftOr = createNode('+');
            leftOr->left = root->left->left;
            leftOr->right = copyTree(root->right);

            Node* rightOr = createNode('+');
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
// Add this new function to task6.c
Node* convertToCNF(Node* root) {
    // Make a copy to avoid destroying the original tree
    Node* cnfTree = copyTree(root); 

    cnfTree = eliminateImplications(cnfTree);
    cnfTree = moveNotInwards(cnfTree);
    cnfTree = distributeOr(cnfTree);

    return cnfTree; 
}

// Utility: Print CNF (in-order traversal with parentheses)
void printCNF(Node* root) {
    if (!root) return;
    if (root->val == '*' || root->val == '+') printf("(");
    printCNF(root->left);
    printf("%c", root->val);
    printCNF(root->right);
    if (root->val == '*' || root->val == '+') printf(")");
}

/*int main() {
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
}*/
