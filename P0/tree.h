#ifndef TREE_H
#define TREE_H
#include <stdio.h>

typedef struct tree {
    int value;
    char *idInfo;
    struct tree *left;
    struct tree *right;
}Tree;

Tree *buildTree(Tree *root, int v);
void Tree_destroy(Tree *root);

#endif
