#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef TREE_H
#define TREE_H

struct node {
    char *string;
    int freq;
    char *code;
    struct node *left;
    struct node *right;
};

int stringContains(char this, char *string);
struct node *find(char this, struct node *htree);
struct node *createNode(char *string, int freq, char *code, struct node *left,
                        struct node *right);
void freeNode(struct node *node);
#endif