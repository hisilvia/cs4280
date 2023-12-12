#ifndef IDENTENTRY_H
#define IDENTENTRY_H
#include <stdbool.h>

#include "token.h"

typedef struct idEntry {
      Token te;
      struct idEntry *next;
}IdEntry;

typedef struct idList {
      int size; 
      IdEntry *first;
}IdList;

void insert(Token t);
bool verify(Token t);
void lookup(Token t);
#endif
