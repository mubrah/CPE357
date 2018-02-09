#include "tree.h"

struct node *createNode(char *string, int freq, struct node *left, struct node *right) {
    struct node *newNode = NULL;

    if ((newNode = malloc(sizeof(*newNode))) != NULL) {
        newNode->string = string;
        newNode->freq = freq;
        newNode->left = left;
        newNode->right = right;
    } else {
        /* Handle NULL */
    }
    return newNode;
}

void freeNode(struct node *node) {
    free(node->string);
    free(node->code);
    if (node->left != NULL) {
        freeNode(node->left);
    }
    if (node->right != NULL) {
        freeNode(node->right);    
    }
    free(node);
}