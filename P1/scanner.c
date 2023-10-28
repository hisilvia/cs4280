//scanner.c
//Resources: K. C. Louden. Chapter 2 “Compiler Construction Principles and Practice”, CENGAGE, 1997
//https://www.geeksforgeeks.org/fseek-in-c-with-example/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "scanner.h"

const int MAXKEYWORDS = 13;
char *keywords[] = {"start", "stop", "loop", "void", "var", "end", "scan",
                     "print", "main", "cond", "then", "let", "func"};

char *tokenNames[] = {"Identifier", "Number", "Keyword", "Operator", "Delimiter",
                      "COMMENT", "EOFtk","UNDERSCORE","ERROR"};
static int mode;
static int lineno = 0;
static char word[100];
static int wordIndex = 0;
static Token currentToken;
static char str[100];
static int linepos = 0;
static int bufsize = 0;
static char lessEq[3];
static char greEq[3];
static int tokenInsIndex = 0;
//-------------------------------------------------------
static char getNextChar(){
    if (!(linepos < bufsize)) {
        lineno++;
        if (fgets(str, sizeof(str), source)) {
             bufsize = strlen(str);
             //printf("bufsize = %d\n", bufsize);
             linepos=0;
             return str[linepos++];
        }else return EOF;
   }else return str[linepos++];

}
//--------------------------------------------------------------
static void ungetNextChar() {
   linepos--;
}
//--------------------------------------------------------------
static void checkKeywords(){
   int i;
   for (i = 0; i < MAXKEYWORDS; i++) {
        if (currentToken.type == IDENT_tk ) {
           if ( strcmp(keywords[i], currentToken.instance) == 0)
                currentToken.type = KW_tk;
        }
   }
}
//*------------------------------------------------------------
static void checkRemain(char s){
         switch (s) {
             case '=':
             case '+':
             case '-':
             case '*':
             case '/':
             case '%':
                mode = 3;
                break;
             case '~':
             case ':':
             case '.':
             case '(':
             case ')':
             case ',':
             case '{':
             case '}':
             case ';':
             case '[':
             case ']':
                mode = 4;
                break;

             default:
                mode = 5;
                break;
         }
}
//-------------------------------------------------------------
static void tokenResult() {
   if (feof(source)) {
         currentToken.type = EOF_tk;
         strcpy(currentToken.instance, "EOF");
         currentToken.lineno = lineno;
   }else if (mode == 1) {
         currentToken.type = IDENT_tk;
         strcpy(currentToken.instance, word);
        // printf("currentTokn.instance-ID = %s\n",currentToken.instance);
         currentToken.lineno = lineno;
   }else if (mode == 2) {
         currentToken.type = NUM_tk;
         strcpy(currentToken.instance, word);
         currentToken.lineno = lineno;
   }else if (mode == 3) {
         currentToken.type = OP_tk;
         strcpy(currentToken.instance, word);
         currentToken.lineno = lineno;
   }else if (mode == 31) {
         currentToken.type = OP_tk;
         strcpy(currentToken.instance, lessEq);
         currentToken.lineno = lineno;
   }else if (mode == 32) {
         currentToken.type = OP_tk;
         strcpy(currentToken.instance, greEq);
         currentToken.lineno = lineno;
   }else if (mode == 4) {
         currentToken.type = DE_tk;
         strcpy(currentToken.instance, word);
         currentToken.lineno = lineno;
   }else if (mode == 5) {
         currentToken.type = ERROR_tk;
         strcpy(currentToken.instance, word);
         currentToken.lineno = lineno;
   }else if (mode == 6) {
         currentToken.type = COMM_tk;
         strcpy(currentToken.instance,"\0");
         currentToken.lineno = lineno;
   }

}
//-------------------------------------------------------------
static char filter() {
   char c = getNextChar();
   if (feof(source)){
         currentToken.type = EOF_tk;
    }
    if (isspace(c) || c == '\n') {
       word[wordIndex] = '\0';

    }else if (isalpha(c)){
        word[wordIndex++] = c;

    }else if (isdigit(c)){
        word[wordIndex++] = c;
    }else if (c == '#') {
        do {
            c = getNextChar();
        }while( c != '#');
        word[wordIndex++] = '\0';
        c = '\0';
    }else {
        switch (c) {
             case '=':
             case '<':
             case '>':
             case '+':
             case '-':
             case '*':
             case '/':
             case '%':
                //word[wordIndex++] = c;
                //break;
             case '~':
             case ':':
             case '.':
             case '(':
             case ')':
             case ',':
             case '{':
             case '}':
             case ';':
             case '[':
             case ']':

                word[wordIndex++] = c;
                break;

             default:
                word[wordIndex++] = c;
                break;
        }
   }
   return c;
}
//--------------------------------------------------------------
static Token scanner() {
   char currentChar = filter();

   if (isalpha(currentChar)) {
          currentChar = filter();
          mode = 1;
          while (isalpha(currentChar))
               currentChar = filter();
          if (!isalpha(currentChar) && !isspace(currentChar)) {
               mode = 5;
               do{
                      currentChar =filter();
               }while (!isspace(currentChar));
          }

   }
   else if (isdigit(currentChar)) {
          currentChar =filter();
          mode = 2;
          while (isdigit(currentChar))
               currentChar = filter();
          if (!isdigit(currentChar) && !isspace(currentChar)) {
               mode = 5;
               do{
                      currentChar =filter();
               }while (!isspace(currentChar));
          }
   }
   else if (isspace(currentChar) || currentChar == '\n') {
          currentChar = filter();
          if (isalpha(currentChar)){
               mode = 1;
               while(isalpha(currentChar))
                      currentChar = filter();
               if (!isalpha(currentChar) && !isspace(currentChar)) {
                      mode = 5;
                      do{
                           currentChar =filter();
                      }while (!isspace(currentChar));
          }

          }else if (isdigit(currentChar)) {
               mode = 2;
               while(isdigit(currentChar))
                      currentChar = filter();
               if (!isdigit(currentChar) && !isspace(currentChar)) {
                      mode = 5;
                      do{
                             currentChar =filter();
                      }while (!isspace(currentChar));
               }
          }else if (currentChar == '<') {
                currentChar = filter();
                if (currentChar == '=' ) {
                       mode = 31;
                       strcpy(lessEq,word);
                }else {
                       mode = 3;
                       ungetNextChar();
                       word[wordIndex] = '\0';
                }
          }else if (currentChar == '>') {
                currentChar = filter();
                if (currentChar == '=') {
                       mode = 32;
                       strcpy(greEq,word);
                }else {
                       mode = 3;
                       ungetNextChar();
                       word[wordIndex] = '\0';
                }
        }else if (currentChar == '\0') {
                 mode = 6;
                 word[wordIndex++] = '\0';
         }else {
                 checkRemain(currentChar);
         }
   }else if (currentChar == '<') {
                 currentChar = filter();
                 if (currentChar == '=' ) {
                        mode = 31;
                        strcpy(lessEq,word);
                 }else {
                        mode = 3;
                        ungetNextChar();
                        word[wordIndex] = '\0';
                 }
   }else if (currentChar == '>') {
              currentChar = filter();
              if (currentChar == '=') {
                    mode = 32;
                    strcpy(greEq,word);
              }else {
                    mode = 3;
                    ungetNextChar();
                    word[wordIndex] = '\0';
              }

  }else if (currentChar == '\0') {
              mode =6;
              word[wordIndex++] = '\0';
   }else {
              checkRemain(currentChar);
   }

   tokenResult();
   checkKeywords();

   tokenInsIndex = 0;
   wordIndex = 0;

   //Use memset to empty string
   memset(word, '\0', sizeof(word));

   return currentToken;
}

//----------------------------------------------------------------------------
void testScanner() {
   Token t;
   t = scanner();      //get one token

   while (true) {
     if ( feof(source)) {
         printf("%s\n", tokenNames[t.type]);
         break;
     }else if (t.type == COMM_tk)
         ;
     else if (t.type == ERROR_tk)
         printf("LEXICAL ERROR: %s is unknow token in line %d.\n", t.instance, t.lineno);
     else
         printf("%s %s %d\n", tokenNames[t.type], t.instance, t.lineno);

     t = scanner();
   }
}
