#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef TREE_H
#define TREE_H

/* If used for a linked list, right will be used as the "next" side
 * For a doubly linked list, left will be used as the "previous" side
 */
struct node {
    char *string;
    int freq;
    char *code;
    struct node *left;
    struct node *right;
};

struct node *createNode(char *string, int freq, char *code, struct node *left, struct node *right);
void freeNode(struct node *node);
#endif