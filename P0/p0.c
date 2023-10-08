//Author   : Silvia Matthews
//Course   : CS4280
//Date     : 9/28/2023
//Prof.    : Cezary Janikow, Ph.D
//Reference: https://copyprogramming.com/howto/what-is-isatty-in-c-for
//         : Yung-Hsiang Lu, Intermediate C Programming

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "tree.h"

#define MAX_INPUT_SIZE 100
Tree *root = NULL;
FILE *fp1, *fp2, *fp3;

//****************************Function Prototype***********************
static void exitError(const char *s);
static void validateData(const char s[]);
static void readKeyboardInput(Tree *root);
static void readFileInput(Tree *tn, char *fn);
static void printInorder(Tree *root, int);
static void printPreorder(Tree *root, int);
static void printPostorder(Tree *root, int);

//******************************Main Function****************************
int main(int argc, char **argv) {

  if (argc > 2) exitError("Too many arguments");

  if (argc == 2) {
     //Read input from file
     readFileInput(root, argv[1]);
  }else if (isatty(STDIN_FILENO) ) {
     //Read input from keyboard
     readKeyboardInput(root);
  }

  int level = 0;

  //Open three output files
  fp1 = fopen("out.preorder", "w+");
  if (fp1 == NULL) {
      printf("fopen fail.\n");
      return EXIT_FAILURE;
   }

  fp2 = fopen("out.inorder", "w+");
  if (fp2 == NULL) {
      printf("fopen fail.\n");
      return EXIT_FAILURE;
   }

  fp3 = fopen("out.postorder", "w+");
  if (fp3 == NULL) {
      printf("fopen fail.\n");
      return EXIT_FAILURE;
   }

  //Call functions
  printPreorder(root, level);
  printf("Creating a file: out.preorder.\n");

  printInorder(root, level);
  printf("Creating a file: out.inorder.\n");

  printPostorder(root, level);
  printf("Creating a file: out.postorder.\n");

  //Close files
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);

  //Free
  Tree_destroy(root);

  return EXIT_SUCCESS;
}

//********************************Functions*********************************
static void exitError(const char *s) {
  printf("%s",s);
  exit(1);
}
//-------------------------------------------------------------------------
static void validateData(const char s[]) {
  int i;
  for (i = 0; i < strlen(s); i++) {
      if (!(isdigit(s[i]))) {
         printf("Data contains non-digit character\n");
         exit (1);
      }else if (strlen(s) < 2) {
         printf("All numbers should be greater than 10.\n");
         exit (1);
      }
  }
}
//------------------------------------------------------------------------
static void readKeyboardInput(Tree *tn) {
  printf("Needs to read from keyboard....\n");
  char input[MAX_INPUT_SIZE];

  while (fscanf(stdin,"%s",input) == 1) {
       validateData(input);
       printf("Input: %s\n", input);
       root = buildTree(root, atoi(input));
  }
}
//------------------------------------------------------------------------
static void readFileInput(Tree *tn, char *filename) {
    printf("Needs to read from file: %s.f1\n", filename);
    char input[MAX_INPUT_SIZE];

    FILE *fptr = fopen(strcat(filename,".f1"), "r");
    if (fptr == NULL) {
        perror("Unable to open file");
        printf("Check a file\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fptr, "%s", input) == 1) {
       validateData(input);
       root = buildTree(root, atoi(input));
    }

    fclose(fptr);
}
//-----------------------------------------------------------------------
static void printPreorder(Tree *tn, int level) {
   if (tn == NULL) return;

   fprintf(fp1, "%*c%d:%-9s ", level*2, ' ', tn->value, tn->idInfo);
   fprintf(fp1, "\n");
   printPreorder(tn -> left, level+1);
   printPreorder(tn -> right, level+1);
}
//-----------------------------------------------------------------------
static void printInorder(Tree *tn, int level) {
   if (tn == NULL) return;

   printInorder(tn -> left, level+1);
   fprintf(fp2, "%*c%d:%-9s ", level*2, ' ', tn->value, tn->idInfo);
   fprintf(fp2, "\n");
   printInorder(tn -> right, level+1);
}
//-----------------------------------------------------------------------
static void printPostorder(Tree *tn, int level) {
   if (tn == NULL) return;

   printPostorder(tn -> left, level+1);
   printPostorder(tn -> right, level+1);
   fprintf(fp3, "%*c%d:%-9s ", level*2, ' ', tn->value, tn->idInfo);
   fprintf(fp3, "\n");
}
