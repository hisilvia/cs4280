//scanner.c
//Resources: K. C. Louden. Chapter 1 “Compiler Construction Principles and Practice”, CENGAGE, 1997

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

//char *tokenNames[] = {"Identifier", "Number", "Operator", "Delimiter", "WS", 
  //                    "EOFtk","UNDERSCORE","ERROR","COMMENT","Keyword"};

typedef enum {
     
     s1, s2, s3, s4, s5, ERROR,FINAL
     
}stateType;

int Table[5][7] = {
  // L      D      OP     DE     WS    EOF    _
  // ---------------------------------------------------
  { s2,    s3,    s4,    s5,    s1,   FINAL, ERROR }, //0
  { s2,   FINAL, FINAL, FINAL, FINAL, FINAL,   s2  }, //1 
  {FINAL,  s3,   FINAL, FINAL, FINAL, FINAL, FINAL }, //2
  {FINAL, FINAL,  s4,   FINAL, FINAL, FINAL, FINAL }, //3
  {FINAL, FINAL, FINAL,  s5,   FINAL, FINAL, FINAL }, //4 

};

static char word[9];
static int wordIndex = 0;
static char str[100];
static int linepos = 0;
static int bufsize = 0;
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
//-------------------------------------------------------------
static int filter() {

   int result;
   char c = getNextChar();
   if (feof(source)){
        result = ENDOFFILE;
    }else if (isspace(c)) {
        //word[wordIndex++] = c;   
        result = WS;  
    }else if (isalpha(c)){
        word[wordIndex++] = c;
        result = L;
    }else if (isdigit(c)){
        word[wordIndex++] = c;
        result = D;
    }else if (c == '#') {
        do {
            c = getNextChar();
            if (feof(source)) {
                printf("LEXICAL ERROR: Not found 2nd # for comment.\n");
                exit(EXIT_FAILURE);
            }
        }while( c != '#');
        if (c == '#') {
             ;
        }
     
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
                word[wordIndex++] = c;
                result = OP;
                break;
       
             case '_':
                word[wordIndex++] = c;
                result = UNDERSCORE;
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
                word[wordIndex++] = c;
                result = DE;
                break;
             
             default:   //Error
                word[wordIndex++] = c;
                result = ERRORF;
                printf("LEXICAL ERROR: %c is unknow token in line %d.\n", c, lineno);
                exit(ERROR);

        }//end of switch
   }//end of else
  
   return result;
}
//--------------------------------------------------------------
Token scanner() { 

   stateType state = s1;
 
   Token currentToken;

   while (state != FINAL) {
         int c = filter();
	 
	 switch (state) {
	  case s1:
	      if (c == L)
		      state = s2;
	      else if (c == D)
		      state = s3;
              else if (c == OP)
		      state = s4;
	      else if (c == DE)
		      state = s5;
	      else if (c == WS) {
		      state = s1;
	      }else if (c == ENDOFFILE)
		      state = FINAL;
                      currentToken.type = EOF_tk;
              break;
		  
	  case s2:
	      if (( c != L) && (c != UNDERSCORE) ){
		      ungetNextChar();
		      state = FINAL;
		      currentToken.type = IDENT_tk;
                      if (c != WS)   
                         word[strlen(word) -1] = '\0';
		      strcpy(currentToken.instance, word);
		      currentToken.lineno = lineno;
	      }
	      break;
		  
	  case s3:
	      if ( c != D ) {
                      ungetNextChar();
		      state = FINAL;
		      currentToken.type = NUM_tk;
                      if (c != WS)
                         word[strlen(word) -1] = '\0';   
		      strcpy(currentToken.instance, word);
		      currentToken.lineno = lineno;
	      }
	      break;
	 
	  case s4:
	      if ( c != OP ) {
                      ungetNextChar();
		      state = FINAL;
		      currentToken.type = OP_tk;
                      if (c !=WS)
                        word[strlen(word) -1] = '\0';   
		      strcpy(currentToken.instance, word);
		      currentToken.lineno = lineno;
	      }
	      break;
	  
	  case s5:
	      if ( c != DE ) {
                      ungetNextChar();
		      state = FINAL;
		      currentToken.type = DE_tk;  
                      if (c != WS)
                           word[strlen(word) -1] = '\0'; 
		      strcpy(currentToken.instance, word);
		      currentToken.lineno = lineno;
	      }
	      break;
	 
	  case FINAL:
	  default:
	      state = FINAL;
	      currentToken.type = ERROR_tk;
	      currentToken.lineno = lineno;
	      break;
	 
	 }//end of switch
	 
	 if (state == FINAL) {
	     word[wordIndex] = '\0';
		 if (currentToken.type == IDENT_tk) {
		     int i;
                     for (i = 0; i < MAXKEYWORDS; i++) {
                         if ( strcmp(keywords[i], currentToken.instance) == 0)
                            currentToken.type = KW_tk;
                     }
                  }    
          }
   }//end of while

   wordIndex = 0;

   //Use memset to empty string
   memset(word, '\0', sizeof(word));
      
   return currentToken;
}

/*----------------------------------------------------------------------------
void testScanner() {
   Token t;
   t = scanner();      //get one token
   
   while (true) {    
     if ( feof(source)) {
         printf("%s\n", tokenNames[t.type]);
         break;
     }else if (t.type == ERROR_tk) 
         printf("LEXICAL ERROR: %s is unknow token in line %d.\n", t.instance, t.lineno);
     else 
         printf("%s %s %d\n", tokenNames[t.type], t.instance, t.lineno);
     
     t = scanner(); 
   }
}*/
