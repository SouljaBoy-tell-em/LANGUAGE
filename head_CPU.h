#include <limits.h>
#include <stdint.h>
#include <stdarg.h>
#include "foundation.h"
#include "head_stack.h"



int addingInStack (CPU * cpu);
void CPUctor (CPU * cpu, int amount_commands);
int createRAM (int ** ramElements);
void createRegisters (Register * registers);
void detectPush (int * commandsArray, int * ip, Register * registers, Stack * stack, int * ramElements);
void endProgram (CPU * cpu);
int exploreRegister (char * arg, Register * registers);
unsigned long FileSize (FILE * compfile);
void InitializeCommandsArray (int ** commandsArray, int amount_commands);
unsigned int InitializeStructRegistersArray (Register ** registers);