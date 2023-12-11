//generator.c  not on stack
//generator.c - temporary value not at stack
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "testTree.h"
#include "processIdent.h"
#include "generator.h"

int j = 0;                  //Hold the number of global variables
Item iniVar[6];             //Create a object of struct Item

static int labelCntr = 0;   //Counting unique labels generated
static int varCntr = 0;     //Counting unique temporaries generated
static char name[20];       //For creation of unique names

static char *newName(nameType what) {
     if (what==VAR)         //Creating new temporary
          sprintf(name, "T%d", varCntr++);
     else                   //Creating new label
          sprintf(name, "L%d", labelCntr++);

     return name;
}

static void recGen(TreeNode *nodeP, FILE *out) {    //Recursivve traversal
     char label[20], argR[20];                      //Local storage for temporary
     char label2[20];                               //Hold the loop-Inlabel2

     if (nodeP==NULL)    return;                    //No nodes

     switch (nodeP->label) {                        //Perform different actions based on the node label         
          //<program>      ->      <vars> main <stats> end
          case PROGRAMty:
                 recGen(nodeP->child[0],out);
                 //printf("finish <vars>\n");
                 recGen(nodeP->child[1],out);
                 break;

          //<vars>         ->     empty | let <varList> .
          case VARSty:
                 if (nodeP->child[0] != NULL) {
                          recGen(nodeP->child[0],out);
                          //printf("finish child<varList> in <vars>\n");
                 }
                 break;

          //<varList>      ->     id = integer <X1>
          case VARLISTty:
                 strcpy(iniVar[j].ins,nodeP->token1.instance);
                 //printf("iniVar.ins= %s\n",iniVar[j].ins);
                 iniVar[j].value = atoi(nodeP->token2.instance);
                 //printf("iniVar.value= %d\n",iniVar[j].value);
                 j++;

                 recGen(nodeP->child[0],out);

                 break;

          //<X1>           ->      empty | <varList>
          case X1ty:
                 if (nodeP->child[0] != NULL) {
                          recGen(nodeP->child[0],out);
                          //printf("work on child in <x1>\n");
                 };
                 break;

          //<exp>          ->      <M> + <exp> | <M> - <exp> | <M>
          case EXPty:
                 if (strcmp(nodeP->token1.instance,"+") == 0 || strcmp(nodeP->token1.instance,"-") == 0 ) {
                           recGen(nodeP->child[1],out);
                           strcpy(argR, newName(VAR));
                           fprintf(out, "STORE %s\n", argR);
                           recGen(nodeP->child[0],out);
                           if (strcmp(nodeP->token1.instance,"+") == 0) {
                                    fprintf(out, "ADD %s\n", argR);
                           }else if (strcmp(nodeP->token1.instance,"-") == 0) {
                                    fprintf(out, "SUB %s\n", argR);
                           }
                 }else {
                           recGen(nodeP->child[0],out);
                           //fprintf(out, "STORE %s\n", argR);
                           printf("finish <M> inside <exp>\n");
                           return;
                 }
                 break;

          //<M>            ->      <N> * <M> | <N>
          case Mty:
                 if (strcmp(nodeP->token1.instance,"*") == 0) {
                           recGen(nodeP->child[1],out);
                           strcpy(argR, newName(VAR));
                           fprintf(out, "STORE %s\n",argR);
                           recGen(nodeP->child[0],out);
                           fprintf(out, "MULT %s\n",argR);
                 }else {
                           recGen(nodeP->child[0],out);
                           printf("done on only one child <N> in <M>\n");
                 }
                 break;

          //<N>            ->      <R> / <N> | -<N> | <R>
          case Nty:
                 if (strcmp(nodeP->token1.instance,"-") == 0) {
                           recGen(nodeP->child[0],out);
                           strcpy(argR, newName(VAR));
                           fprintf(out, "STORE %s\n",argR);
                           fprintf(out, "MULT -1\n");

                 }else if (strcmp(nodeP->token1.instance,"/") == 0) {
                           recGen(nodeP->child[1],out);
                           strcpy(argR, newName(VAR));
                           fprintf(out, "STORE %s\n",argR);
                           recGen(nodeP->child[0],out);
                           fprintf(out, "DIV %s\n", argR);

                 }else {   recGen(nodeP->child[0],out);
                           printf("only one child <R> in <N>\n");
                 }
                 break;

          //<R>            ->      [<exp>] | id | integer
          case Rty:
                 if (nodeP->token1.type == DE_tk) {
                           recGen(nodeP->child[0],out);
                           return;
                 }else {fprintf(out, "LOAD %s\n",nodeP->token1.instance);
                       printf("Load id or # in <R>\n");
                 }
                 break;

          //<stats>        ->      <stat> <mStat>
          case STATSty:
                 recGen(nodeP->child[0],out);
                 printf("finish 1st child in <stats>\n");
                 recGen(nodeP->child[1],out);
                 break;

          //<mStat>        ->      empty | <stat> <mStat>
          case MSTATty:
                 if (nodeP->child[0] != NULL) {
                              recGen(nodeP->child[0],out);
                              printf("finish 1st child in <mStat>\n");
                              recGen(nodeP->child[1],out);
                 }else return;
                 printf("finish <mStat>\n");
                 break;

          //<stat>         ->      <in> | <out> | <block> | <if> | <loop> | <assign>
          case STATty:
                 recGen(nodeP->child[0],out);
                 printf("finish <stat>\n");
                 break;

          //<block>        ->      start <vars> <stats> stop
          case BLOCKty:
                 recGen(nodeP->child[0],out);
                 printf("Done on <vars> in <block>\n");
                 recGen(nodeP->child[1],out);
                 break;

          //<in>           ->      scan identifier .
          case INty:
                 printf("inside <in>\n");
                 printf("READ %s\n",nodeP->token1.instance);
                 fprintf(out, "READ %s\n",nodeP->token1.instance);

                 break;


          //<out>          ->      print <exp> .
          case OUTty:
                 recGen(nodeP->child[0],out);
                 strcpy(argR, newName(VAR));
                 fprintf(out, "STORE %s\n",argR);
                 fprintf(out, "WRITE %s\n",argR);
                 break;

          //<if>           ->      cond ( <exp> <RO> <exp> ) <stat>
          case IFty:
                 recGen(nodeP->child[2],out);
                 strcpy(argR, newName(VAR));
                 fprintf(out, "STORE %s\n", argR);

                 recGen(nodeP->child[0],out);
                 fprintf(out, "SUB %s\n", argR);
                 strcpy(label, newName(LABEL));

                 //Look up tokens from <RO>
                if (strcmp(nodeP->child[1]->token1.instance,"<") == 0) {
                       fprintf(out, "BRZPOS %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,">") == 0) {
                       fprintf(out, "BRZNEG %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,"=") == 0) {
                       fprintf(out, "BRPOS %s\nBRNEG %s\n", label,label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,"~") == 0) {
                       fprintf(out, "BRZERO %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,"<=") == 0) {
                       fprintf(out, "BRPOS %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,">=") == 0) {
                       fprintf(out, "BRNEG %s\n", label);
                 }
                 recGen(nodeP->child[3],out);
                 fprintf(out, "%s: NOOP\n", label);

                 break;

          //<loop>         ->      loop ( <exp> <RO> <exp> ) <stat>
          case LOOPty:
                 strcpy(label2, newName(LABEL));
                 fprintf(out, "In%s: ", label2);

                 recGen(nodeP->child[2],out);
                 strcpy(argR,newName(VAR));
                 fprintf(out, "STORE %s\n", argR);

                 recGen(nodeP->child[0],out);
                 fprintf(out, "SUB %s\n", argR);
                 strcpy(label, newName(LABEL));

                 //Look up tokens from <RO>
                if (strcmp(nodeP->child[1]->token1.instance,"<") == 0) {
                       fprintf(out, "BRZPOS %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,">") == 0) {
                       fprintf(out, "BRZNEG %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,"=") == 0) {
                       fprintf(out, "BRPOS %s\nBRNEG %s\n", label,label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,"~") == 0) {
                       fprintf(out, "BRZERO %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,"<=") == 0) {
                       fprintf(out, "BRPOS %s\n", label);

                 }else if (strcmp(nodeP->child[1]->token1.instance,">=") == 0) {
                       fprintf(out, "BRNEG %s\n", label);
                 }
                 recGen(nodeP->child[3],out);
                 fprintf(out, "BR In%s\n", label2);
                 fprintf(out, "%s: NOOP\n", label);


                 break;

          //<assign>       ->      id ~ <exp> .
          case ASSIGNty:
                 recGen(nodeP->child[0],out);
                 fprintf(out,"STORE %s\n",nodeP->token1.instance);
                 break;

          //<RO>           ->      <= | >= | < | > | = | ~

          default:

                 break;
     }


}
//*****************************************************************************************
void generator(TreeNode *gn, FILE *out) {
   if (gn == NULL) {
        printf("Generator Error: Invalid parse tree recieved.\n");
        exit(1);
   }
   printf("start generator\n");
   recGen(gn,out);
   printf("finish generator\n");
   fprintf(out, "STOP\n");
   int i;
   for (i=0; i<varCntr;i++) {
        fprintf(out, "T%d 0\n", i);
   }
   for (i=0; i<j;i++) {
        fprintf(out, "%s %d\n",iniVar[i].ins,iniVar[i].value);
   }
}
//*****************************************************************************************
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
//****************************************************************************************
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
//***************************************************************************************
void staticSemantics(TreeNode *tn) {
   if (tn == NULL)    return;

   traverse1(tn);
   traverse2(tn);
   printf("staticSemantics complited\n");
}
