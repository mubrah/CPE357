#include "tree.h"

int stringContains(char this, char *string) {
    int i;
    for (i = 0; string[i]; i++) {
        if (this == string[i])
            return 1;
    }
    return 0;
}

struct node *find(char this, struct node *htree) {
    if (htree->string[0] == this) {
        return htree;
    } else {
        if (stringContains(this, htree->left->string)) {
            return find(this, htree->left);
        } else {
            return find(this, htree->right);
        }
    }
}

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