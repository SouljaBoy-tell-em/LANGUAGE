#include <ctype.h> 
#include <math.h>
#include "foundation.h"


unsigned int amountOfString (char * mem, unsigned long filesize);
void closeMemoryPointers (FILE * compFile, FILE * fileDecompilation, FILE * binaryFile, char * mem_start,\
                          char ** getAdress, int * commandsArray, Label * labels, Register * registers);
void compile (int * commandsArray, char ** getAdress, unsigned long amount_of_strings, Label * labels, Register * registers);
bool createCommandsArray (int ** bufferNumberCommands, unsigned long amount_of_strings, Label ** labels);
void createRegisters (Register * registers);
//void decompilation (int * commandsArray, Label * labels, FILE * fileDecompilation, unsigned long sizeCommandsArray, Register * registers);
//void decompilationCommand (int command, FILE * fileDecompilation, int * flagDualCommands, Register * registers);
int detect2ndLabel (char * getAdress, Label * labels);
int exploreRegister (char * arg, Register * registers);
unsigned long FileSize (FILE * file);
void getAssemblerCommands (char * capacityBuffer, int * commandsArray, char * getAdress, Label * labels, int numString, Register * registers, int * j);
unsigned int getBuffer (char ** mem_start, unsigned long filesize,\
                            unsigned long * amount_of_string, FILE * file);
unsigned int get2ndArg (char * getAdress, Register * registers, int * commandArray, Label * labels);
void InititalizeCommandsArray (Label * labels);
unsigned int InitializePointersArray (char *** getAdress, char * mem_start, unsigned long filesize,\
                              unsigned long amount_of_string);
unsigned int InitializeStructRegistersArray (Register ** registers);
void jumpExam (int * commandsArray, Label * labels, char ** getAdress, int amount_of_strings);
void pointerGetStr (char * buffer, char ** getAdress, unsigned long filesize);
bool popEmptyArg (char * getAdress, int * commandsArray);
void recordInBuffer (char * mem_start);