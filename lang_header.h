#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


char * s = NULL;


enum Type {

	EMPTY,
	NUMBER,
	OPERATION,
	VARIABLE,
	CIRCBRACKETS,
	CONDITION,
	FUNCTION,
	NAMEFUNCTION,
	STATEMENT,
	PARAM,
	CLOSE = -228
};


#define LENGTHVALUE 512

#define ADD    '+'
#define SUB    '-'
#define MUL    '*'
#define DIV    '/'
#define DEG    '^'

#define J      '='       + 0
#define JA     '>'       + 0
#define JB     '<'       + 0
#define JE     '=' + '=' + 0
#define JAE    '<' + '=' + 0
#define JBE    '>' + '=' + 0
#define JNE    '!' + '=' + 0

#define IF     105 + 102
#define COS    99  + 111 + 115
#define SIN    115 + 105 + 110
#define DEF    100 + 101 + 102
#define END    101 + 110 + 100
#define OUT    111 + 116 + 117
#define CALL   99  + 97  + 108 + 108
#define SQRT   115 + 113 + 114 + 116
#define PRINT  112 + 114 + 105 + 110 + 116
#define START  115 + 116 + 97  + 114 + 116
#define WHILE  119 + 104 + 105 + 108 + 101
#define RETURN 114 + 101 + 116 + 117 + 114 + 110

#define EMBEDIF()																			\
		  rightPartOfInitializeVariable (node->left->left,  commands);		\
		  rightPartOfInitializeVariable (node->left->right, commands);		\
		  																						\
		  determineJmp (node->left, commands);										\
		  fprintf (commands, ":");														\
		  int saveNumLabel = addingNewLabel (node->left, commands);			\
		  fprintf (commands, "\n");													\
		  rightPartOfInitializeVariable (node->right, commands);				\
		  fprintf (commands, "start%d:\n", saveNumLabel);						\
		  																						\
		  node->type = CLOSE;															

#define EMBEDWHILE()																		\
		rightPartOfInitializeVariable (node->left->left,  commands);		\
		rightPartOfInitializeVariable (node->left->right, commands);		\
																								\
		int saveNumLabel = addingNewLabel (node->left, commands);			\
		fprintf (commands, ":\n");														\
																								\
		rightPartOfInitializeVariable (node->right, commands);				\
		fprintf (commands, "push %d\n", node->left->right->code);			\
		determineJmp (node->left, commands);										\
		fprintf (commands, ":start%d\n", saveNumLabel);							\
																								\
		node->type = CLOSE;

#define EMBEDPRINT()																		\
		rightPartOfInitializeVariable (node->right, commands);				\
		fprintf (commands, "print\n");												\
																								\
		node->type = CLOSE;																

#define EMBEDFUNCTIONSQRT()															\
		rightPartOfInitializeVariable (node->left, commands);					\
																								\
		node->type = CLOSE;


#define EMBEDINITIALIZEVARIABLE()													\
		rightPartOfInitializeVariable (node->parent->right, commands);		\
		fprintf (commands, "pop r%cx\n", node->code);							\
																								\
		node->type = CLOSE;																\

#define CHECK_ERROR(condition, message_error) 		       \
            do {                                          \
               if (condition) {                           \
                   printf ("%s", message_error);          \
                   exit (EXIT_FAILURE);                   \
               }                                          \
            } while (false)


typedef struct node {

	node *    left;
	node *   right;
	node *  parent;
	enum Type type;
	int       code;
	char name[LENGTHVALUE];
} Node;


Node *        constructor             (void);
unsigned long FileSize                (FILE * file);
void          graphDump               (Node * currentNode, FILE * graphDumpFile);
void          graphDumpDrawNode       (Node * currentNode, FILE * graphDumpFile, Node * prevNode);
char *        InitializeS             (FILE * langFile, int sizelangFile);
int           InitializeTree          (Node * node, char * s, int sizelangFile);
int           listFunction            (int code);
void          listingTxtCode          (Node * node, int sizelangFile);
Type          listMainWords           (Node * node);
void          SkippingOfSpacesSymbols (char ** s);


// ROOLS:
#include "frontend_header.h"
#include "frontend.h"


// CONVERT TREE IN COMMANDS FILE:
#include "backend_header.h"
#include "backend.h"