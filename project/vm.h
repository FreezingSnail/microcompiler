#ifndef vm_h 
#define vm_h 

#include "chunk.h"
#include "common.h"

typedef struct {
    int vars[3]; //a-0 b-1 c-3 
    bool defined[3];
} vars;

 typedef struct {  
  Chunk* chunk;
  uint8_t* ip;

  int stack[256];
  int* stackTop;  

    vars variableList;
  
} VM; 



void initVM();    
void freeVM();  

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

InterpretResult interpret(const char* source);
void push(int value);                 
int pop();  



#endif 