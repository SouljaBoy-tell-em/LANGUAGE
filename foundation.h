#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


#define TURNOFFMASKPOP 								 ~(1 << 27)
#define MASKPOP 									   1 << 27
#define MASKIMMED 									   1 << 28
#define TURNOFFMASKIMMED 							 ~(1 << 28)
#define MASKREGISTER 								   1 << 29
#define TURNOFFMASKREGISTER 						 ~(1 << 29)
#define MASKRAM 									   1 << 30
#define TURNOFFMASKRAM 								 ~(1 << 30)
#define TURNOFFMASKRAMANDIMMED 		(~(1 << 30)) ^ (~(1 << 28))
#define TURNOFFMASKRAMANDREGISTER 	(~(1 << 30)) ^ (~(1 << 29))
#define ALLTURNOFF ((~(1 << 29)) ^ (~(1 << 28)) ^ (~(1 << 27)))

#define AMOUNTCOMMANDS 1000
#define AMOUNTLABELS 30
#define AMOUNTREGISTERS 25
#define CANARY 0xDEDDED32
#define INTRAMELEMENTS 400
#define LENREGISTER 5
#define MAXLEN 512
#define MAXLENCOMMAND 100
#define POISON -228
#define RATIO_SIZE_STACK 2
#define SIGNATURE 0xDEDDED32
#define STACKSIZE 50
#define VERSION 1


#define CHECK_ERROR(condition, message_error) \
            do {                                          \
               if (condition) {                           \
                   printf ("%s", message_error);          \
                   return EXIT_FAILURE;                   \
               }                                          \
            } while(false)

#define MAIN_DET(detector) if (detector) {                \
                            exit (EXIT_FAILURE);          \
                            }

#define DEF_CMD(name, num, arg, ...) \
                CMD_##name = num,

#define DEF_JMP(name, num, ...) 	 \
                CMD_##name = num,


enum COMMANDS {

    #include "cmd.h"
};

#undef DEF_CMD
#undef DEF_JMP

#define STACKNAME(INIT, var) strcpy (INIT, (#var))

#define STATUS(codeError) (codeError) ? "error": "ok"


typedef int Elem_t;

typedef struct {

	long long startStructCanary;
	char name [MAXLEN];
	Elem_t * data;
	int size;
	int capacity;
	unsigned int code_of_error;
	long long hash_stack;
	Elem_t hash_data;
	long long finishStructCanary;
} Stack;

typedef struct {

    char name [LENREGISTER];
    int equationRegister;
} Register;

typedef struct {

    char name [MAXLENCOMMAND];
    int numberCommand;
    int numberString;
} Node;

typedef struct {

    Node arrayLabels [AMOUNTLABELS];
    int amountLabels;
} Label;


typedef struct {

	Stack stack;
	int * commandsArray;
	Register * registers;
	int * ramElements;
	int amountCommands;
	int ip;
} CPU;


enum error_memory {

    FILE_AREN_T_OPENING = 1,
    MEMORY_NOT_FOUND,
    EMPTY_FILE,
    BAD_EQUATION
};

enum errors {

	NO_ERROR = 0,
	BAD_DATA = 1 << 1,
	BAD_SIZE = 1 << 2,
	BAD_CAPACITY = 1 << 3,
	STACKOVERFLOW = 1 << 4,
	BAD_START_ELEM_T_CANARY = 1 << 5,
	BAD_FINISH_ELEM_T_CANARY = 1 << 6,
	BAD_START_STRUCT_CANARY = 1 << 7,
	BAD_FINISH_STRUCT_CANARY = 1 << 8,
	STACK_NULL = 1 << 9
};