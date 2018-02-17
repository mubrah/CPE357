#include "tree.h"

struct node *createNode(char *string, int freq, char *code, struct node *left,
                        struct node *right) {
    struct node *newNode = NULL;

    if ((newNode = malloc(sizeof(*newNode))) == NULL) {
        /* Handle NULL */
    } else {
        newNode->string = string;
        newNode->freq = freq;
        newNode->code = code;
        newNode->left = left;
        newNode->right = right;
    }
    return newNode;
}

void freeNode(struct node *node) {
    if (node != NULL) {
        if (node->string != NULL)
            free(node->string);
        if (node->code != NULL)
            free(node->code);
        if (node->left != NULL) 
            freeNode(node->left);
        if (node->right != NULL)
            freeNode(node->right);    
        free(node);
    }
}