#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "token.h"
#include "testTree.h"

typedef enum {VAR, LABEL} nameType;

//Hold the definition variables and its values
typedef struct item {
    char ins[9];
    int value;
}Item;

void staticSemantics(TreeNode *t);
void generator(TreeNode *n, FILE *out);

#endif
