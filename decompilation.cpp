#include "foundation.h"


void decompilation (int * commandsArray, FILE * fileDecompilation, int amount_commands);
void decompilationCommand (int * commandsArray, FILE * fileDecompilation, int * j);
unsigned long FileSize (FILE * file);


int main (void) {

    FILE * binaryFile       =                fopen ("binaryFile.bin", "rb");
    CHECK_ERROR (binaryFile == NULL, "Problem with opening binaryFile.txt");

    FILE * fileDecompilation       =           fopen ("afterCompileFile.txt", "w");
    CHECK_ERROR (fileDecompilation == NULL, "Problem with opening binaryFile.txt");

    int amount_commands = FileSize (binaryFile) / sizeof (int);
    int commandsArray [amount_commands];
    fread (commandsArray, sizeof (int),  amount_commands, binaryFile);
    decompilation (commandsArray, fileDecompilation, amount_commands);

	return 0;
}


void decompilation (int * commandsArray, FILE * fileDecompilation, int amount_commands) {

    fprintf (fileDecompilation, "%20s\n", "LISTING");
    fprintf (fileDecompilation, "%10s %10s %10s\n", "num:", "command:", "argument:");

    int i = 0;
    for (i = 2; i < amount_commands; i++) 
        decompilationCommand (commandsArray, fileDecompilation, &i);
}


unsigned long FileSize (FILE * compfile) {

    struct stat buf = {};
    if (fstat (fileno (compfile), &buf) == 0)
        return buf.st_size;

    return 0;
}


void decompilationCommand (int * commandsArray, FILE * fileDecompilation, int * j) {

    int save = commandsArray[( * j)] & ALLTURNOFF;
    static int i = 0;

    fprintf (fileDecompilation, "%10d", i);
    i++;
    #define DEF_CMD(name, num, arg, ...)                                                            \
                case num:                                                                           \
                        if (arg) {                                                                  \
                            fprintf (fileDecompilation, "%10s", #name);                             \
                            ( * j)++;                                                               \
                            fprintf (fileDecompilation, "%10d\n", commandsArray [ ( * j)]);         \
                            return;                                                                 \
                            break;                                                                  \
                        }                                                                           \
                        fprintf (fileDecompilation, "%10s\n", #name);                               \
                        if (num == CMD_hlt)                                                         \
                            exit (EXIT_SUCCESS);                                                    \
                        return;                                                                     \
                        break;                                                                                  

    #define DEF_JMP(name, num, ...)                                                                 \
                    DEF_CMD(name, num, 1)   

    switch (save)  {
            
            #include "cmd.h"                        
            #undef DEF_CMD
            #undef DEF_JMP

            default:
                break;             
    }

}