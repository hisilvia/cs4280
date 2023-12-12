//Author   : Silvia Matthews
//Course   : CS4280
//Project  : P4 - Target code and Storage
//Date     : 12/5/2023
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
#include "generator.h"

TreeNode *root = NULL;
FILE *source;
FILE *target;
int lineno = 0;
//****************************Function Prototype***********************
static void exitError(const char *s);
static void readFileInput(char *fn);
//******************************Main Function****************************
int main(int argc, char **argv) {
  
  if (argc > 2 ) exitError("Too many arguments.");
 
  if (argc == 2) 
       readFileInput(argv[1]);     
  else if (isatty(STDIN_FILENO))
       source = stdin;

  root = parser();
    
  staticSemantics(root);
 
  //Open target output file
  //char *outFile = NULL;
  target = fopen("a.asm", "w+");
  if (target == NULL) {
       printf("target file open failed.\n");
       exit(EXIT_FAILURE);
  }
 
  generator(root,target);

  fclose(source);
  fclose(target);
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
