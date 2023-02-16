unsigned int amountOfString (char * mem, unsigned long filesize) {

    unsigned long indexFile = 0, amount = 0;
    for (indexFile = 0; indexFile < filesize; indexFile++)
        if ( * (mem + indexFile) == '\0')
            amount++;

    return amount + 1;
}


void closeMemoryPointers (FILE * compFile, FILE * fileDecompilation, FILE * binaryFile, char * mem_start,\
                          char ** getAdress, int * commandsArray, Label * labels, Register * registers)  {

    fclose (compFile);
    fclose (fileDecompilation);
    fclose (binaryFile);
    free (mem_start);
    free (getAdress);
    free (commandsArray);
    free (labels);
    free (registers);
}


void compile (int * commandsArray, char ** getAdress, unsigned long amount_of_strings, Label * labels, Register * registers) {

    char capacityBuffer [MAXLENCOMMAND];
    int numString = 0, val = 0, j = 2;

    for (numString = 0; numString < amount_of_strings; numString++) {

        sscanf (getAdress [numString], "%s", capacityBuffer);
        getAssemblerCommands (capacityBuffer, commandsArray, getAdress [numString], labels, numString, registers, &j);
    }
}


bool createCommandsArray (int ** bufferNumberCommands, unsigned long amount_of_strings, Label ** labels) {

    * bufferNumberCommands = (int * ) calloc (2 * amount_of_strings + 2, sizeof (int));
    * labels = (Label * ) calloc (1, sizeof (Label));
    ** bufferNumberCommands = SIGNATURE;
    * ( * bufferNumberCommands + 1) = VERSION;
    
    if (bufferNumberCommands == NULL)
        return false;

    return true;
}


void createRegisters (Register * registers) {

    char startRegister [LENREGISTER] = "rax";

    int i = 0;
    for (i = 0; i < AMOUNTREGISTERS; i++) {

        strcpy ((registers + i)->name, startRegister);
        startRegister [1]++;
    }
}


int detect2ndLabel (char * getAdress, Label * labels) {

    int startLabel = (int) (strchr (getAdress, ':') - getAdress);
    char capacityBuffer [MAXLENCOMMAND];
    strcpy (capacityBuffer, getAdress + startLabel + 1);
    
    int i = 0;
    for (i = 0; i < labels->amountLabels; i++)
        if (strcmp (capacityBuffer, (labels->arrayLabels)[i].name) == 0)
            return (labels->arrayLabels)[i].numberCommand;
 
    return -1;
}


int exploreRegister (char * arg, Register * registers) {

    if ( * arg != 'r' || * (arg + 2) != 'x')
        return -1;

    char secondLetterStartRegister = 'a';

    int i = 0;
    for (i = 0; i < AMOUNTREGISTERS; i++) {

        if ( * (arg + 1) == secondLetterStartRegister)
            return i;

        secondLetterStartRegister++;
    }

    return -1;
}   


unsigned long FileSize (FILE * compfile) {

    struct stat buf = {};
    if (fstat (fileno (compfile), &buf) == 0)
        return buf.st_size;

    return 0;
}


void getAssemblerCommands (char * capacityBuffer, int * commandsArray, char * getAdress, Label * labels, int numString, Register * registers, int * j) {

    int lenNameLabel = strlen (capacityBuffer), val = 0, labelFlag = 0;

    int numColon = (int)(strchr(getAdress, ':') - getAdress);
    if (numColon) {

        if (numColon == strlen (capacityBuffer) - 1) {

            strncpy ((labels->arrayLabels[labels->amountLabels]).name, capacityBuffer, lenNameLabel - 1);
            labels->arrayLabels[labels->amountLabels].numberCommand = *j;
            labels->arrayLabels[labels->amountLabels].numberString = numString;
            labels->amountLabels++;
            return;
        }
    }



    #define DEF_CMD(name, num, arg, ...)                                                        \
                    if (strcmp (capacityBuffer, #name) == 0) {                                  \
                                                                                                \
                        commandsArray [(*j)] = num;                                                \
                        if (arg)  {                                                             \
                            val = get2ndArg (getAdress, registers, &commandsArray [(*j)], labels); \
                            (*j)++;                                                                \
                            commandsArray [(*j)] = val;                                            \
                        }                                                                       \
                        (*j)++;                                                                    \
                        return;                                                                 \
                    }
    #define DEF_JMP(name, num, ...) \
            DEF_CMD(name, num, 1)  

    #include "cmd.h" 
    #undef DEF_CMD
    #undef DEF_JMP
}


unsigned int getBuffer (char ** mem_start, unsigned long filesize,\
                            unsigned long * amount_of_string, FILE * file) {

    * mem_start = (char * ) calloc (filesize, sizeof (char));
    CHECK_ERROR (* mem_start == NULL, "Memory not allocated for mem_start.");
    fread (* mem_start, sizeof (char), filesize, file);
    recordInBuffer (* mem_start);
    * amount_of_string = amountOfString (* mem_start, filesize);

    return NO_ERROR;
}


unsigned int get2ndArg (char * getAdress, Register * registers, int * commandArray, Label * labels) {

    char arg [MAXLENCOMMAND];
    int lenStr = 0, val = 0;

    if (!strcmp (getAdress, "in")) {

        if (scanf ("%d", &val) == 1)
            return val;

        return BAD_EQUATION;
    }

    if (strchr (getAdress, ':')) {

        val = detect2ndLabel (getAdress, labels);
        return val;
    }

    sscanf (getAdress, "%s%n", arg, &lenStr);
    while (isspace ( * (getAdress + lenStr)))
        lenStr++;


    sscanf (getAdress + lenStr, "%s", arg);

    if ( * arg == '[' && * (arg + strlen (arg) - 1) == ']') {

        * (getAdress + lenStr) = * (getAdress + lenStr + strlen (arg) - 1) = ' ';
        * commandArray = ( * commandArray) | MASKRAM;
    }

    if (sscanf (getAdress, "%s %d", arg, &val) == 2) {

        * commandArray = ( * commandArray) | MASKIMMED;
        return val;
    } 

    if (sscanf (getAdress, "%s %s", arg, arg) == 2) {

        if ((val = exploreRegister (arg, registers)) >= 0)
            * commandArray = ( * commandArray) | MASKREGISTER;

        return val;
    }

    return BAD_EQUATION;
}


void InititalizeCommandsArray (Label * labels)  {

    (labels)->amountLabels = 0;
}


unsigned int InitializePointersArray (char *** getAdress, char * mem_start, unsigned long filesize,\
                              unsigned long amount_of_string) {

    * getAdress = (char ** )calloc (amount_of_string, MAXLENCOMMAND);
    CHECK_ERROR (* getAdress == NULL, "Memory not allocated for getAdress.");
    pointerGetStr (mem_start, * getAdress, filesize);

    return NO_ERROR;
}


unsigned int InitializeStructRegistersArray (Register ** registers) {

    * registers = (Register * )calloc (AMOUNTREGISTERS, sizeof (Register));
    CHECK_ERROR (* registers == NULL, "Memory not allocated for registers.");

    return NO_ERROR;
}


void pointerGetStr (char * buffer, char ** getAdress, unsigned long filesize) {

    getAdress [0] = &buffer [0];
    
    unsigned long i = 1, j = 1;
    for (i = 0; i < filesize; i++) {
        if (buffer[i] == '\0') {

            getAdress [j] = &buffer [i+1];
            j++;
        }
    }
}


bool popEmptyArg (char * getAdress, int * commandsArray) {

    char arg [MAXLENCOMMAND];
    int lenStr = 0, val = 0;

    sscanf (getAdress, "%s%n", arg, &lenStr);

    while (isspace ( * (getAdress + lenStr)))
        lenStr++;

    if (lenStr == strlen (getAdress)) {

        * commandsArray = ( * commandsArray) | MASKPOP;
        return false;
    }

    return true;
}


void recordInBuffer (char * mem_start) {

    int amount_of_symbols = strlen (mem_start);
    for (int i = 0; i < amount_of_symbols; i++) {

        if (mem_start[i] == EOF)
            mem_start[i] = '\0';

        if (mem_start [i] == '\n') {

            mem_start[i] = '\0';
            continue;
        }
    }

}