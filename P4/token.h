#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
      IDENT_tk,   //Identifier
      NUM_tk,     //Number
      OP_tk,      //Operator
      DE_tk,      //Delimiter
      WS_tk,
      EOF_tk,
      UNDERSCORE_tk,
      ERROR_tk,
      COMM_tk,
      KW_tk,      //Keyword
      NULL_tk,   
}tokenID;

typedef struct token{
      tokenID type;
      char instance[9];
      int lineno;
}Token;

#endif


