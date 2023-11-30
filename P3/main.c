//Author   : Silvia Matthews
//Course   : CS4280
//Project  : P3 - Static Semantics
//Date     : 11/22/2023
//Prof.    : Cezary Janikow, Ph.D
//Reference: https://copyprogramming.com/howto/what-is-isatty-in-c-for
//         : Yung-Hsiang Lu, Intermediate C Programming

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "testTree.h"

TreeNode *root = NULL;
FILE *source;
int lineno = 0;
//****************************Function Prototype***********************
static void exitError(const char *s);
static void readFileInput(char *fn);
//******************************Main Function****************************
int main(int argc, char **argv) {
  
  if (argc > 2 ) exitError("Too many arguments.");
 
  if (argc == 2) 
       readFileInput(argv[1]);     
  else
       source = stdin;

  //int level = 0;

  root = parser();
  
  //printPreorder(root, level);  
  staticSemantics(root);

  fclose(source);
  TreeNode_destroy(root);

  printf("Finished!\n");
  return EXIT_SUCCESS;
}
//********************************Functions*********************************
static void exitError(const char *s) {
  printf("%s",s);
  exit(1);
}
//------------------------------------------------------------------------
static void readFileInput(char *filename) {
    printf("Needs to read from file: %s.f1\n", filename);
   
    source = fopen(strcat(filename,".f1"), "r");
    if (source == NULL) {
         perror("Unable to open file");
         printf("Check a file\n");
         exit(EXIT_FAILURE);
    }

}
