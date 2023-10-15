//tree.c
//Reference: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

//bool verbose = true;

static Tree *Tree_construct(int val) {
   Tree *tn;
   int temp = val;
   tn = malloc(sizeof(Tree));

   //Convert the value to a string
   char strVal[100];
   sprintf(strVal, "%d", val);

   //Allocate memory for tn->idInfo
   tn -> idInfo = (char *)malloc(strlen(strVal) + 2); //One for space, one for null terminator

   while (temp >= 10) {
       temp = temp/10;
   }

   tn -> value = temp;

   if (temp == tn -> value) {
       strcat(tn->idInfo, " ");
       strcat(tn->idInfo, strVal);
   }
   tn -> left = NULL;
   tn -> right = NULL;

   return tn;
}

Tree *buildTree(Tree *tn, int val) {
   if (tn == NULL) {
      //Empty, create a node
      return Tree_construct(val);
   }

   //Not empty
   int temp = val;
    while (temp >= 10) {
       temp = temp/10;
   }

   //Convert the value to a string
   char strVal[20];
   sprintf(strVal, "%d", val);

   char *tempStr;
   tempStr = (char *)malloc(strlen(tn->idInfo));

   //The original string will be broken into a series of tokens using the delimiter
   //Copy the string into a new string
   strcpy(tempStr,tn->idInfo);
   if (temp == (tn -> value)) {
      //Do not insert the same value
      //Add the number starting with same first digit
      char *token = strtok(tn->idInfo, " ");
      while (token != NULL) {
         if (strcmp(token, strVal) == 0  ) {
              break;
         }else if ( strcmp(token, strVal)!= 0  ) {
              token = strtok(NULL," ");
         }
      }
/* Test if the result is right
      if (verbose) {
          printf("tempStr = %s", tempStr);
      }   
*/
      if (token == NULL) {
          strcat(tempStr, " ");
          strcat(tempStr, strVal);
      }
      strcpy( tn->idInfo,tempStr);

      return tn;
   }

   if (temp < (tn -> value)) {
      tn -> left = buildTree(tn -> left, val);
   }else {
      tn -> right = buildTree(tn -> right, val);
   }

   return tn;
}

void Tree_destroy(Tree *n) {
   if (n == NULL)   return;

   Tree_destroy(n -> left);
   Tree_destroy(n -> right);

   free(n);
}
