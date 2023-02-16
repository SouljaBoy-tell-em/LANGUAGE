int addingInStack (CPU * cpu) {

	int i = 0;
	for (i = 2; i < cpu->amountCommands; i++) {

		int save = cpu->commandsArray[i] & ALLTURNOFF;

		#define DEF_CMD(name, num, arg, ...)      															\
							case num:     		  															\
								__VA_ARGS__		  															\
								break;			  
							

		#define DEF_JMP(name, num, symbol)			  														\
							case num:																		\
								if (i < cpu->commandsArray [i + 1])	{										\
									int save1 = StackPop (&(cpu->stack));									\
									int save2 = StackPop (&(cpu->stack));									\
									if (save2 symbol save1)													\
										i++;																\
									else 																	\
										i = cpu->commandsArray[i + 1] - 1;									\
									break;																	\
								}																			\
								if (i > cpu->commandsArray [i + 1]) {										\
									if ((cpu->registers)->equationRegister symbol StackPop (&(cpu->stack))) \
										i = cpu->commandsArray [i + 1] - 1;		   							\
									break;																	\
								}																			\
								else {																		\
									printf ("\n\n\nTrigger\n\n\n");	\
									i++;																	\
									break;																	\
								}									

		switch (save)  {
			
			#include "cmd.h"						
			#undef DEF_CMD
			#undef DEF_JMP

			default:
				break;
		}
	}

	return 0;
}


void CPUctor (CPU * cpu, int amount_commands) {

	cpu->amountCommands = amount_commands;
	InitializeCommandsArray (&(cpu->commandsArray), cpu->amountCommands);
	StackCtor (&(cpu->stack), STACKSIZE);
	InitializeStructRegistersArray (&(cpu->registers));
	createRAM (&(cpu->ramElements));
	cpu->ip = 0;
}


int createRAM (int ** ramElements) {

	* ramElements = (int * ) calloc (INTRAMELEMENTS, sizeof (int));
	if (ramElements == NULL)
		return MEMORY_NOT_FOUND;

	int i = 0;
	for (i = 0; i < INTRAMELEMENTS; i++)
		* ( * ramElements + i) = 0;

	return NO_ERROR;
}


void createRegisters (Register * registers) {

    char startRegister [LENREGISTER] = "rax";

    int i = 0;
    for (i = 0; i < AMOUNTREGISTERS; i++) {

        strcpy ((registers + i)->name, startRegister);
        (registers + i)->equationRegister = 0;
        startRegister [1]++;
    }
    
}


void detectPush (int * commandsArray, int * ip, Register * registers, Stack * stack, int * ramElements) {

		switch (commandsArray [ * ip] & ALLTURNOFF) {

			case CMD_push:
							if ((commandsArray [ * ip] & TURNOFFMASKIMMED) == CMD_push) 					  {
								( * ip)++; StackPush (stack, 				   				               commandsArray [ * ip]); break;
							}
							if ((commandsArray [ * ip] & TURNOFFMASKREGISTER) == CMD_push) 					  {
								( * ip)++; StackPush (stack,               (registers + commandsArray [ * ip])->equationRegister); break;
							}
							if (((commandsArray [ * ip] & TURNOFFMASKRAM) & TURNOFFMASKIMMED) == CMD_push)    {
								( * ip)++; StackPush (stack, 								 ramElements [commandsArray [ * ip]]); break;
							}
							if (((commandsArray [ * ip] & TURNOFFMASKRAM) & TURNOFFMASKREGISTER) == CMD_push) {
								( * ip)++; StackPush (stack, ramElements [(registers + commandsArray [ * ip])->equationRegister]); break;
							}
			case CMD_pop: 	if ((commandsArray [ * ip] & TURNOFFMASKPOP) == CMD_pop)						  {
								StackPop (stack); 																				   break;
							}
							if ((commandsArray [ * ip] & TURNOFFMASKREGISTER) == CMD_pop) 					  {
								( * ip)++; 				 (registers + commandsArray [ * ip])->equationRegister = StackPop (stack); break;
							}
							if (((commandsArray [ * ip] & TURNOFFMASKRAM) & TURNOFFMASKIMMED) == CMD_pop)     {
								( * ip)++;                                 ramElements [commandsArray [ * ip]] = StackPop (stack); break;
							}
							if (((commandsArray [ * ip] & TURNOFFMASKRAM) & TURNOFFMASKREGISTER) == CMD_pop)  {
								( * ip)++; ramElements [(registers + commandsArray [ * ip])->equationRegister] = StackPop (stack); break;
							}
			case CMD_in:		( * ip)++; StackPush (stack, commandsArray [ * ip]); 						    				   break;
			case CMD_pow: 		( * ip)++; StackPush (stack, pow (StackPop (stack), commandsArray [ * ip]));					   break;
			case CMD_sqrt: 		( * ip)++; StackPush (stack, sqrt (StackPop (stack)));											   break;
			default:																											   break;
		}

		if (commandsArray [ * ip] == CMD_copy) {

			int save = StackPop (stack);
			StackPush (stack, save);
			StackPush (stack, save);
			//( * ip)++;
		}

		if (commandsArray [ * ip] == CMD_print) {

			int save = StackPop (stack);
			StackPush (stack, save);
			printf ("%d ", save);
		}
}


void endProgram (CPU * cpu) {

	printf ("\n\n\n\n\n");

	int i = 0;
    for (i = 0; i < 15; i++)
    	printf ("register r%cx: %d\n", (i + 97), (cpu->registers)[i].equationRegister);

    exit (0);
}


int exploreRegister (char * arg, Register * registers) {

    if ( * arg != 'r' || * (arg + 2) != 'x')
        return POISON;

    char secondLetterStartRegister = 'a';

    int i = 0;
    for (i = 0; i < AMOUNTREGISTERS; i++) {

        if ( * (arg + 1) == secondLetterStartRegister)
            return registers[i].equationRegister;

        secondLetterStartRegister++;
    }

    return POISON;
}   


unsigned long FileSize (FILE * compfile) {

    struct stat buf = {};
    if (fstat (fileno (compfile), &buf) == 0)
        return buf.st_size;

    return 0;
}


void InitializeCommandsArray (int ** commandsArray, int amount_commands) {

	* commandsArray = (int * ) calloc (amount_commands, sizeof (int));

	int i = 0;
	for (i = 0; i < amount_commands; i++)
		 (* commandsArray) [i] = 0;
}


unsigned int InitializeStructRegistersArray (Register ** registers) {

    * registers = (Register * )calloc (AMOUNTREGISTERS, sizeof (Register));
    if (registers == NULL)
    	return MEMORY_NOT_FOUND;

    createRegisters ( * registers);

    return NO_ERROR;
}