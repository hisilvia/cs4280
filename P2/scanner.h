#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include "token.h"

extern FILE *source;
extern int lineno;

typedef enum {
    L,D,OP,DE,WS,ENDOFFILE,COMMENT,UNDERSCORE,ERRORF

}numOfCol;

Token scanner();

#endif
