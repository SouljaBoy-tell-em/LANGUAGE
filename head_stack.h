void dumpFileCleaning (void);
void errorsDecoder (Stack * stack, FILE * dump);
unsigned int fullCodeError (Stack * stack);
uint8_t * getStartData (Stack * stack);
void StackClear (Stack * stack);
void StackCtor (Stack * stack, int capacity);
void StackDump (Stack * stack, int lineStackDump, const char * nameFunctionDump, const char * fileFunctionDump);
void StackError (Stack * stack);
void StackInfoDump (Stack * stack, FILE * dump);
Elem_t StackPop (Stack * stack);
void StackPush (Stack * stack, Elem_t addDataElement);
void StackReSizeDown (Stack * stack);
void StackReSizeUp (Stack * stack, Elem_t addDataElement);
void UninititalizeElements (Stack * stack);


