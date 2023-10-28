#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
      IDENT_tk,   //Identifier
      NUM_tk,     //Number
      KW_tk,      //Keyword
      OP_tk,      //Operator
      DE_tk,      //Delimiter
      COMM_tk,
      EOF_tk,
      UNDERSCORE,
      ERROR_tk
}tokenID;

typedef struct token{
      tokenID type;
      char instance[9];
      int lineno;
}Token;

#endif
