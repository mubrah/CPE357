#include "tree.h"

int cmpNodes(const void *_wc1, const void *_wc2) {
    struct node *wc1, *wc2;

    wc1 = *(const struct node **)_wc1;
    wc2 = *(const struct node **)_wc2;
    if (wc1->freq == wc2->freq) {
        return strcmp(wc2->string, wc1->string);      
    } else {
        return wc2->freq - wc1->freq;
    }
}

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
    free(node->left);
    free(node->right);
    free(node);
}