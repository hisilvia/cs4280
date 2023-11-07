#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include "token.h"

extern FILE *source;

typedef enum {
    L,D,OP,DE,WS,ENDOFFILE,COMMENT,UNDERSCORE,ERRORF

}numOfCol;

void testScanner();

#endif

