#include "head_CPU.h"
#include "CPU.h"
#include "stack.h"

int main (void) {

	FILE * binaryFile 		=                 fopen ("binaryFile.bin", "rb");
	CHECK_ERROR (binaryFile == NULL, "Problem with opening compileFile.txt");

	CPU cpu = {};

	int amount_commands = FileSize (binaryFile) / sizeof (int);
	int commandsArray [amount_commands];

	CPUctor (&cpu, amount_commands);
	fread (cpu.commandsArray, sizeof (int),  cpu.amountCommands, binaryFile);
    addingInStack (&cpu);

	return 0;
}