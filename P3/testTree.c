//testTree.c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "testTree.h"
#include "processIdent.h"

//In C99, you can use a designated initializer to initialize a structure:
//Token tk1
//tk1 = (Token) {IDENT_tk,"",0 };

char *tokenNames[] = {"Identifier","Number","Operator","Delimiter","WS","EOFtk",
                      "UNDERSCORE","ERROR","Comment","Keyword","NULL"};

char *labelNames[] = {"program","vars","varList","X1","exp","M","N","R","stats","mStat",
                      "stat","block","in","out","if","loop","assign","RO"};

//************************************************************************
TreeNode *getNewNode(labelType label) {
   TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
   int i;

   //Initialize the token
   Token tk1 = (Token) {NULL_tk,"",lineno};
   Token tk2 = (Token) {NULL_tk,"",lineno};

   if (t==NULL) {
       printf("Out of memory error at line %d\n",lineno); 
       exit(1);
   }else {
       for (i=0; i<MAXCHILDREN;i++)  t->child[i] = NULL;          
       t->label = label;
       t->token1 = tk1;
       t->token2 = tk2;
   }
   return t;
}
//**************************************************************************
void printPreorder(TreeNode *tn, int level) {
     if (tn == NULL) return;

     int i;
     printf("%*c%s ", level*2, ' ',labelNames[tn->label]);

     if (tn->token1.type != NULL_tk)
         printf("%s %s %d ", tokenNames[tn->token1.type], tn->token1.instance, tn->token1.lineno);

     if (tn->token2.type != NULL_tk)
         printf("%s %s %d ", tokenNames[tn->token2.type], tn->token2.instance, tn->token2.lineno);
 
     printf("\n");
     for (i=0; i<MAXCHILDREN; i++)
           printPreorder(tn->child[i], level+1);
 
}
//****************************************************************************
void TreeNode_destroy(TreeNode *n) {
   if (n == NULL)   return;

   int i;
   for (i=0; i<MAXCHILDREN;i++)
          TreeNode_destroy(n->child[i]);

   free(n);
}
//*******************************************************************************
static void traverse1(TreeNode *n) {
   if (n == NULL) return;

   if (n->label == VARLISTty) {
           if (n->token1.type == IDENT_tk) 
                 insert(n->token1); 
   }

   int i;
   for (i=0; i<MAXCHILDREN; i++)
           traverse1(n->child[i]);

}
//******************************************************************************
static void traverse2(TreeNode *n) {
   if (n == NULL) return;

   if (n->label != VARSty) {
   //if (n->label == Rty || n->label == INty || n->label == ASSIGNty) {   //This line is equivalent to the above line.
          if (n->token1.type == IDENT_tk) {
                if (!verify(n->token1))
                     printf("Error: variable %s in line %d is not defined\n",n->token1.instance, n->token1.lineno);
          }
    }

   int i;
   for (i=0;i<MAXCHILDREN;i++)
          traverse2(n->child[i]);
}
//******************************************************************************
void staticSemantics(TreeNode *tn) {
   if (tn == NULL)    return;
   
   traverse1(tn);
   traverse2(tn);
}

