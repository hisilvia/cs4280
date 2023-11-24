//testTree.h
#ifndef TESTTREE_H
#define TESTTREE_H
#include <stdio.h>
#include "token.h"


#define MAXCHILDREN 4
typedef enum {

    PROGRAMty,VARSty,VARLISTty,X1ty,EXPty,Mty,Nty,Rty,STATSty,MSTATty,
    STATty,BLOCKty,INty,OUTty,IFty,LOOPty,ASSIGNty,ROty,

}labelType;

typedef struct treeNode {
    labelType label;
    Token token1;
    Token token2;
    struct treeNode *child[4];
}TreeNode;


TreeNode *getNewNode(labelType label); 
void printPreorder(TreeNode *t, int l);
void TreeNode_destroy(TreeNode *t);

#endif
