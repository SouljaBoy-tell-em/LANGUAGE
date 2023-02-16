#include "head_asm.h"
#include "asm.h"


int main (void) {

    FILE * compFile       =                 fopen ("compileFile.txt", "r");
    CHECK_ERROR (compFile == NULL, "Problem with opening compileFile.txt");

    FILE * fileDecompilation       =                 fopen ("afterCompileFile.txt", "w");
    CHECK_ERROR (fileDecompilation == NULL, "Problem with opening afterCompileFile.txt");

    unsigned long filesize = FileSize (compFile), amount_of_strings = 0;
    CHECK_ERROR (filesize  == 0,           "The compilefile is empty.");

    char     *  mem_start     = NULL, 
             ** getAdress     = NULL;
    int      *  commandsArray = NULL;
    Label    *  labels        = NULL;
    Register * registers      = NULL;

    CHECK_ERROR (!createCommandsArray (&commandsArray          , amount_of_strings, &labels), "Problem with allocating memory for commandsArray.");
    MAIN_DET (getBuffer (&mem_start, filesize, &amount_of_strings, compFile)                                                                     );
    CHECK_ERROR (InitializePointersArray (&getAdress, mem_start, filesize, amount_of_strings), "Problem with allocating memory for getAdress."   );
    CHECK_ERROR (InitializeStructRegistersArray (&registers)   ,                               "Problem with allocating memory for registers."   );
    
    createRegisters (registers                                             );
    pointerGetStr (mem_start, getAdress,                           filesize);
    InititalizeCommandsArray (labels);
    compile (commandsArray, getAdress, amount_of_strings, labels, registers);
    compile (commandsArray, getAdress, amount_of_strings, labels, registers);

    FILE * binaryFile       =                fopen ("binaryFile.bin", "wb");
    CHECK_ERROR (binaryFile == NULL, "Problem with opening binaryFile.txt");
    fwrite (commandsArray, sizeof (int), 2 * amount_of_strings, binaryFile);

    int  i = 0;
    for (i = 0; i < 2 * amount_of_strings; i++)
        printf("%d ", commandsArray[i]);


    closeMemoryPointers (compFile, fileDecompilation, binaryFile, mem_start, \
                        getAdress, commandsArray, labels, registers       );
    
    return 0;
}