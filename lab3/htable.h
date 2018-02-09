#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef TREE_H
#include "tree.h"
#endif

#ifndef WORDS_H
#include "words.h"
#endif

#ifndef HTABLE_H
#define HTABLE_H

void appendString(char *existing, char *toAppend, char *buffer);
void getCodes(struct node *HTree, struct node **finalCodes, char *bitString);
int main(int argc, char **argv);
#endif

