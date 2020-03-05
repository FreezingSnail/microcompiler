#include <stdio.h> 

#include "vm.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"

VM vm; 

static int getVar(char c){

    printf("getvar: %c,  %d \n", c, c);
    switch(c){
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        default:
            printf("bad var access\n");
            return -1;
    }
}

static bool defineCheck(int index){
    return vm.variableList.defined[index];
}

static void setVar(int index, int num){
    vm.variableList.vars[index] = num;
    vm.variableList.defined[index] = true;
}
static int getvarValue(int index){
    return vm.variableList.vars[index];
}

static void define(int index){
    vm.variableList.defined[index] = true;
}

static void resetStack() {
  vm.stackTop = vm.stack; 
} 

void push(int value) {
  *vm.stackTop = value; 
  vm.stackTop++;        
}   

int pop() {         
  vm.stackTop--;      
  return *vm.stackTop;
}  


void initVM() {    
    resetStack();
}                  

void freeVM() {    
}   

static void printStack(){
for (int* slot = vm.stack; slot < vm.stackTop; slot++) {      
      printf("[ ");                                                 
      printValue(*slot);                                            
      printf(" ]");                                                 
    } 
}

                                      

static InterpretResult run() {          
#define READ_BYTE() (*vm.ip++)          
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])


#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)  

  for (;;) {      

     printf("          ");                                           
    for (int* slot = vm.stack; slot < vm.stackTop; slot++) {      
      printf("[ ");                                                 
      printValue(*slot);                                            
      printf(" ]");                                                 
    }                                                               
    printf("\n");    
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
    printf("test\n");              
    uint8_t instruction;                
    switch (instruction = READ_BYTE()) {
        case OP_VAR:
        case OP_CONSTANT: {                
            int constant = READ_CONSTANT();
            printValue(constant); 
            
            push(constant);           
            printf("\n");                    
            break;                           
      }    
      case OP_DOT: {                 
        return INTERPRET_OK; 
      }    
      case OP_ADD:      BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE:   BINARY_OP(/); break;  

      case OP_PRINT: {
            printf("Printing: %d",pop());
            printf("\n");
        break;
      }/*
      case OP_DEFINE: {
                ObjString* name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
*/
            case OP_SET: {
                int num = pop();
                int varIndex = getVar((char)pop());
                //if(defineCheck(varIndex))
                setVar(varIndex, num);

                
                
                break;
            } 
            
            case OP_GET:{
                int varIndex = getVar((char)pop());
                if(defineCheck(varIndex))
                    push(getvarValue(varIndex));
                else{
                    printf("var not initialized\n");
                }
            }              
    }                                   
  }                                     

#undef READ_BYTE 
#undef READ_CONSTANT   
#undef BINARY_OP                    
} 

InterpretResult interpret(const char* source) {
  Chunk chunk;                                 
  initChunk(&chunk);

  if (!compile(source, &chunk)) {              
    freeChunk(&chunk);                         
    return INTERPRET_COMPILE_ERROR;            
  }                                            

  vm.chunk = &chunk;                           
  vm.ip = vm.chunk->code;  

    printf("running\n");
  InterpretResult result = run();              
    printf("done\n");
  //freeChunk(&chunk);                           
  return result;                              
}  