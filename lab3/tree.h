#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef TREE_H
#define TREE_H

struct node {
    char *string;
    int freq;
    struct node *left;
    struct node *right;
};

int cmpNodes(const void *_wc1, const void *_wc2);
struct node *createNode(char *string, int freq, struct node *left, struct node *right);
void freeNode(struct node *node);
#endif