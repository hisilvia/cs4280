//parser.c
//Resources: K. C. Louden. Chapter 1 “Compiler Construction Principles and Practice”, CENGAGE, 1997
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "testTree.h"

static Token tk;    //holds current token

//*********************function prototypes for recursive calls************
static TreeNode *program();
static TreeNode *vars();
static TreeNode *varList();
static TreeNode *X1();
static TreeNode *expr();
static TreeNode *M();
static TreeNode *N();
static TreeNode *R();
static TreeNode *stats();
static TreeNode *mStat();
static TreeNode *stat();
static TreeNode *block();
static TreeNode *in();
static TreeNode *out();
static TreeNode *ifif();
static TreeNode *loop();
static TreeNode *assign();
static TreeNode *RO();


static void syntaxError(char *message) {
     printf("\n>>> ");
     printf("Syntax error at line %d: %s\n", lineno, message);
     exit(1);
}

TreeNode *parser() {
     TreeNode *root1;
     tk = scanner();
     root1 = program();
     
     if (!feof(source))
          syntaxError("EOFtk expected");

     printf("Passed the parser successfully\n");
     return root1;
}

TreeNode *program() {
     TreeNode *p = getNewNode(PROGRAMty);
     p->child[0] = vars();
    
     if (strcmp(tk.instance,"main") == 0) {
           //printf("main received in program\n");
           tk = scanner();
                         
           if (strcmp(tk.instance,"scan") == 0 || strcmp(tk.instance,"print") == 0
               || strcmp(tk.instance,"start") == 0 || strcmp(tk.instance,"cond") == 0
                || strcmp(tk.instance,"loop") == 0 || tk.type == IDENT_tk) {
                      
                       if (tk.type == IDENT_tk)
                           p->token1 = tk;

                       p->child[1] = stats();
 
                       if (strcmp(tk.instance,"end") == 0) {
                               tk = scanner();

                               return p;
                       }else syntaxError("NOT found end in <program>");
           
           }else syntaxError("Not found FIRST(stats)={start,scan,print,...} after main");
     }else syntaxError("Not found main keyword");
     
     return p;
}

TreeNode *vars() {
     TreeNode *v = getNewNode(VARSty);
     
     if (strcmp(tk.instance,"let") == 0) {
           tk = scanner();
           v->child[0] = varList();
           //return v;
           if (strcmp(tk.instance,".") == 0) {
                tk = scanner();
                return v;
           }else syntaxError("dot . expected in <vars>");
        
           return v;
     }else { 
           //printf("return ok 1st pro->empty in vars\n");    
           return v;
     }
}

TreeNode *varList() {
     TreeNode *va = getNewNode(VARLISTty);
     if (tk.type == IDENT_tk) {
           va->token1 = tk;
           tk = scanner();
           
           if (strcmp(tk.instance,"=") == 0) {
                tk = scanner();
                //printf("%s in varList\n",tk.instance);
                
                if (tk.type == NUM_tk) {
                         va->token2 = tk;
                         tk = scanner();
                         //printf("%s in varList\n",tk.instance);

                         va->child[0] =  X1();
                         
                         //printf("return ok in varList\n");
                         return va;
                }else syntaxError("integer expected in <varList>");
                
           }else syntaxError("= expected in <varList>");
     }else syntaxError("id expected in <varList>");
     
   return va;
}

TreeNode *X1() {
     TreeNode *x1 = getNewNode(X1ty);
   
     if (tk.type == IDENT_tk) {      
           //x1->token1 = tk;
           x1->child[0] = varList();
           return x1;
     }else {
           return NULL;
     }
}

TreeNode *expr() {
     TreeNode *e = getNewNode(EXPty);
     e->child[0] = M();
     
     if (strcmp(tk.instance,"+") == 0) {
           e->token1 = tk;
           tk = scanner();
           e->child[1] = expr();  
           //printf("return ok. 1st pro in expr\n");
           return e;

     }else if (strcmp(tk.instance,"-") == 0) {
           e->token1 = tk;
           tk = scanner();
           e->child[1] = expr();
           //printf("return ok. 2nd pro in expr\n");
           return e;

     }else {
           //printf("return ok. 3rd pro->empty in expr\n");
           return e;
     }
     
}

TreeNode *M() {
     TreeNode *m = getNewNode(Mty);
     m->child[0] = N();
     //return m;
     if (strcmp(tk.instance,"*") == 0) { 
           m->token1 = tk;       
           tk = scanner();
           
           if (strcmp(tk.instance,"-") == 0 || strcmp(tk.instance,"[") == 0
                || tk.type == IDENT_tk || tk.type == NUM_tk) {
                         //printf("tk.instance = %s in M\n",tk.instance);
                         //m->token2 = tk;
                         m->child[1] = M();
                         //printf("return ok. 1st pro in M\n");
                         return m;

           }else syntaxError("Not found FIRST(N)={-,[,ID,INT} after *");
           //printf("get %s after * in M\n",tk.instance);
           return m;
      }else {
            //printf("return ok. 2nd pro in M\n");
            return m;
      }
      //return m;    
}                  
                 
TreeNode *N() {
     TreeNode *n = getNewNode(Nty);       
     if (strcmp(tk.instance,"-") == 0) {
                n->token1 = tk;  
                tk = scanner();
                //printf("newToken=%s after - in N\n",tk.instance);
                n->child[0] = N();
                //printf("return ok.2nd pro in N\n");
                return n;
     }else {    
                n->child[0] = R();

                if (strcmp(tk.instance,"/") == 0) {
                          n->token1 = tk;

                          //printf("new token = %s after R in N\n",tk.instance);
                          tk = scanner();
                          //printf("another new token = %s after R in N\n",tk.instance);
                          n->child[1] = N();
                           
                          //printf("return ok. 1st pro in N\n");
                          return n;
                 }else { 
                          //printf("tk.instance = %s after R in N\n",tk.instance);     
                          //printf("return ok. 3rd pro->empty in N\n");
                          return n;
                 }

                 //printf("token=%s after else R in N\n",tk.instance);
     }
     //return n;    
}

TreeNode *R() {
     TreeNode *r = getNewNode(Rty);
     
     if (strcmp(tk.instance,"[") == 0 ) {
                r->token1 = tk;
                tk = scanner();
                //printf("new token after [ = %s in R\n",tk.instance);
                r->child[0] = expr();
                              
                if (strcmp(tk.instance,"]") == 0 ) {
                        r->token2 = tk;
                        tk = scanner();
                        //printf("return ok.1st pro in R\n");
                        return r;

                }else syntaxError("] expected in <R>");

                //printf("return ok. 1st pro in R\n");
     }else if (tk.type == IDENT_tk) {
                r->token1 = tk;
                tk = scanner();
                //printf("return ok. 2nd pro in R\n");
                return r;

     }else if (tk.type == NUM_tk) {
                //r->token.type = NUM_tk;
                r->token1 = tk;
                tk = scanner(); 
                //printf("tk.instance = %s after number in R\n",tk.instance);
                //printf("return ok. 3rd pro in R\n");
                return r;

     }//else syntaxError("nothing expected in R");
     return r;
}

TreeNode *stats() {
     TreeNode *s = getNewNode(STATSty);
     s->child[0] = stat();
  
     //printf("tk.instance after stat() in stats = %s\n",tk.instance);
     
     if (strcmp(tk.instance,"scan") == 0 || strcmp(tk.instance,"print") == 0
         || strcmp(tk.instance,"start") == 0 || strcmp(tk.instance,"cond") == 0
         || strcmp(tk.instance,"loop") == 0 || tk.type == IDENT_tk) {
                //printf("received tk.instance = %s in stats\n", tk.instance);
   
                if (tk.type == IDENT_tk) 
                     s->token1 = tk;
   
                s->child[1] = mStat();
                
                    //printf("return ok after mStat() in stats\n");
                return s;
     }else {
                //printf("return ok. only has stat since mStats is empty in stats\n");
                return s;
     }
     //return s;
}

TreeNode *mStat() {
     TreeNode *m = getNewNode(MSTATty);

     if (strcmp(tk.instance,"scan") == 0 || strcmp(tk.instance,"print") == 0
         || strcmp(tk.instance,"start") == 0 || strcmp(tk.instance,"cond") == 0
         || strcmp(tk.instance,"loop") == 0 || tk.type == IDENT_tk) {
                //printf("tk.instance before stat() in mStat = %s\n",tk.instance);
                if (tk.type == IDENT_tk) 
                     m->token1 = tk;

                m->child[0] = stat();
                
                //printf("tk.instance before new token in mStat = %s\n",tk.instance);
                if (strcmp(tk.instance,"scan") == 0 || strcmp(tk.instance,"print") == 0
                     || strcmp(tk.instance,"start") == 0 || strcmp(tk.instance,"cond") == 0
                     || strcmp(tk.instance,"loop") == 0 || tk.type == IDENT_tk) {
                           if (tk.type == IDENT_tk) 
                                 m->token2 = tk;
                           
                           m->child[1] = mStat();
                
                           //printf("tk.instance after mStat in mStat = %s\n",tk.instance);
                           return m;    
                }else {
                           //printf("return ok. pro->0 in mStat\n");
                           return m;
                }
                //return m;
                
     }else {
                //printf("return ok. 1st pro->empty in mStat\n");    
                return NULL;
     }
     //return m;
}
 

TreeNode *stat() {
     TreeNode *s = getNewNode(STATty);
     if (strcmp(tk.instance,"scan") == 0) {
                s->child[0] = in();
                tk = scanner();
                //printf("token = %s in stat \n",tk.instance);
                //printf("return ok. 1st pro in stat\n");
                return s;               
     }else if (strcmp(tk.instance,"print") == 0) {
                //printf("print recieved in stat\n");
                s->child[0] = out();
                
                //printf("token = %s after print in stat\n",tk.instance);
                //printf("return ok. 2nd pro in stat\n");
                return s;
     }else if (strcmp(tk.instance,"start") == 0) {
                s->child[0] = block();
                tk = scanner();
                //printf("token = %s after block in stat\n",tk.instance);
                //printf("return ok. 3rd pro in stat\n");
                return s;
     }else if (strcmp(tk.instance,"cond") == 0) {
                //printf("token=%s in cond in stat\n",tk.instance);
                s->child[0] = ifif();
                
                //printf("token = %s after cond in stat\n",tk.instance);
                //printf("return ok. 4th pro in stat\n");
                return s;
     }else if (strcmp(tk.instance,"loop") == 0) {
                s->child[0] = loop();
                
                //printf("return ok. 5th pro in stat\n");
                return s;
     }else if (tk.type == IDENT_tk) {
                s->token1 = tk;
                
                s->child[0] = assign();
                tk = scanner();
                //printf("Token = %s in IDENTtk in stat\n",tk.instance);
                //printf("return ok. 6th pro in stat\n");
                return s;
     }else {syntaxError("Not found FIRST(stat)={start,scan,...} in <stat>");
            return NULL;}
     //return s;

}

TreeNode *block() {
     TreeNode *b = getNewNode(BLOCKty);
     if (strcmp(tk.instance, "start") == 0) {
                 tk = scanner();
                 //printf("token = %s after start in block \n",tk.instance);
                 b->child[0] = vars(); 
                 b->child[1] = stats();
                 //return b;
                 if (strcmp(tk.instance,"stop") == 0) {
                         //return b;
                         ;//printf("token = %s after stop in block\n",tk.instance);
                         //printf("return ok. at stop in block\n");
                 }else syntaxError("stop except in <block>");

     }else syntaxError("start expected in <block>");
     return b;
}

TreeNode *in() {
     TreeNode *i = getNewNode(INty);
     if (strcmp(tk.instance,"scan") == 0) {
                 tk  = scanner();
                 //return i;
                 //printf("token = %s in in \n",tk.instance);
                 if (tk.type == IDENT_tk) {
                        i->token1 = tk;
                     
                         tk = scanner();
                        //printf("token = %s after ident_tk in in \n",tk.instance);
                         //return i;
                         if (strcmp(tk.instance,".") == 0) {                        
                                     //printf("return ok. in in\n");
                         }else syntaxError("dot . expected at end of <in> ");
                 }else syntaxError("IDtk expected in <in>");
     }else syntaxError("scan expected in <in>");
     return i;
}

TreeNode *out() {
     TreeNode *o = getNewNode(OUTty);
     if (strcmp(tk.instance,"print") == 0) {
                 //printf("print received in out\n");
                 tk  = scanner();
                 //printf("tk.instance = %s before expr() in out\n",tk.instance);
                 o->child[0] = expr();
                 //return o;

                 if (strcmp(tk.instance,".") == 0) {
                          //printf("dot . received in out\n");
                          tk = scanner();
                          //printf("tk.instance = %s after expr() in out\n",tk.instance);
                          //printf("return ok. in out\n");
                           return o;                          
                 }else syntaxError("dot . expected at end in <out>");
                
     }else syntaxError("print expected in <out>");
     return o;
}

TreeNode *ifif() {
     TreeNode *i = getNewNode(IFty);
     if (strcmp(tk.instance,"cond") == 0) {
                 tk = scanner();
                 //printf("Token=%s after cond in if\n",tk.instance); 
                 //return i;
                 if (strcmp(tk.instance,"(") == 0) {
                          tk = scanner();
                          //printf("Token=%s after ( in if\n",tk.instance);
                          i->child[0] = expr();                         
                          i->child[1] = RO();
                          i->child[2] = expr();
                          //return i;
                          if (strcmp(tk.instance,")") == 0) {
                                  tk = scanner();
                                  //printf("Token=%s after ) in if\n",tk.instance);
                                  i->child[3] = stat();
                                  //return i;
                                  //printf("return ok. in if\n"); 
                          }else syntaxError(") expected in <if>");
                 }else syntaxError("( expected in <if>");
     }else syntaxError("cond expected in <if>");
     return i;
}

TreeNode *loop() {
     TreeNode *l = getNewNode(LOOPty);
     if (strcmp(tk.instance,"loop") == 0) {
                 tk = scanner();
                 //return l;
                 if (strcmp(tk.instance,"(") == 0) {
                          tk = scanner();
                          l->child[0] = expr();
                          l->child[1] = RO();
                          l->child[2] = expr();

                          //return l;
                          if (strcmp(tk.instance,")") == 0) {
                                  tk = scanner();
                                  l->child[3] = stat();
                        
                                  //return l;
                          }else syntaxError(") expected after loop in <loop>");
                 }else syntaxError("( expected after loop in <loop>");
     }else syntaxError("loop expected in <loop>");
     return l;
}

TreeNode *assign() {
     TreeNode *a = getNewNode(ASSIGNty);
     if (tk.type == IDENT_tk) {
                 a->token1 = tk;             
                 tk = scanner();
  
                 if (strcmp(tk.instance,"~") == 0) {
                          tk = scanner(); 
                          a->child[0] = expr();
                          //return a;
                          if (strcmp(tk.instance,".") == 0) {
                                   //return a;
                                  ;
                          }//else syntaxError("dot . expected in assign");
                 }else syntaxError("~ expected in <assign>.");
     }else syntaxError("IDtk expected in <assign>");
     return a;
}

TreeNode *RO() {
     TreeNode *r = getNewNode(ROty);
     if (strcmp(tk.instance,"<") == 0 ) {
                 r->token1 = tk;             
                 tk = scanner();
                 return r;              
     }else if (strcmp(tk.instance,">") == 0) {
                 r->token1 = tk;
                 tk = scanner();
                 return r;
     }else if (strcmp(tk.instance,"=") == 0) {
                 r->token1 = tk;
                 tk = scanner();
                 return r;
     }else if (strcmp(tk.instance,"~") == 0) {
                 r->token1 = tk;
                 tk = scanner();
                 return r;
     }else if (strcmp(tk.instance,"<=") == 0) {
                 r->token1 = tk;
                 tk = scanner();
                 return r;
     }else if (strcmp(tk.instance,">=") == 0) {
                 r->token1 = tk;
                 tk = scanner();
                 return r;
     }
     return r;
}
