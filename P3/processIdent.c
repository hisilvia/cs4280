//processIdent.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "processIdent.h"


//Initialize
static IdList id = (IdList) {0,NULL};

//*********************************************************************************************
char *lookup(Token t) {
   char *result = NULL;
   result = (char *) malloc(sizeof(t.instance));
   int i;
   IdEntry *curId = id.first;

   for (i = 0; i<id.size; i++) {
         if (strcmp(curId->te.instance, t.instance) == 0) {
               printf("Error: multiply defined variable %s in line %d\n",t.instance,t.lineno);
               exit(1);
         }
         curId = curId->next;
   }
   strcpy(result,t.instance);
   printf("No mult variables with same name\n");
   return result;
}
//********************************************************************************************
void insert(Token t) {
   IdEntry *newID = (IdEntry *) malloc (sizeof(IdEntry));
   //printf("coming inside insert()\n");
  
   char *newChar = lookup(t);
   strcpy(newID->te.instance, newChar);
   newID->te = t;
   newID->next = id.first;
   id.first = newID;
   id.size++;
   printf("insert variable %s in line %d\n",newID->te.instance, newID->te.lineno);
   
   while (newID != NULL) {
        printf("%s ", newID->te.instance);
        newID = newID->next;
   }
   printf("\n\n");

}
//**********************************************************************************************
bool verify(Token t) {
   IdEntry *ptr = id.first;
   while (ptr != NULL) {
        if (strcmp(ptr->te.instance, t.instance) == 0) {
                    printf("The variable %s in line %d is used after definition.\n",t.instance,t.lineno);
                    return true;
        }else {
                    ptr = ptr -> next;                    
        }
   }
   
   if (ptr == NULL) {
        //printf("Error: variable %s in line %d is not defined.\n",t.instance, t.lineno);
        return false;
   }
   return NULL;   
}

