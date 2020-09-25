#include <stdio.h> 

#include "vm.h"
#include "common.h"

#include "compiler.h"



VM vm; 

static void runtimeErrorCall(){
    printf("exception\n");
    vm.runtimeError = true;
}

static int getVar(char c){
    #ifdef DEBUGGER
        printf("getvar: %c,  %d \n", c, c);
    #endif
    switch(c){
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        default:
           // printf("bad var access\n");
           runtimeErrorCall();
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
    if(defineCheck(index))
        return vm.variableList.vars[index];
    else
        runtimeErrorCall();
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

static void resetVars(){
    for(int i=0;i<3;i++){
    vm.variableList.vars[i] = 0;
    vm.variableList.defined[i] = false;
    }
}


void initVM() {  
    vm.runtimeError = 0;  
    resetStack();
    resetVars();
}     

void reset(){
    initVM();
}

void freeVM() {    
    //freeChunk(vm.chunk);
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

    uint8_t instruction;  
    if(vm.runtimeError){
        vm.runtimeError = false;
        return INTERPRET_RUNTIME_ERROR;   

    }           
    switch (instruction = READ_BYTE()) {
        case OP_VAR:
        case OP_CONSTANT: {                
            int constant = READ_CONSTANT();
            #ifdef DEBUGGER
                printValue(constant);
                printf("\n");
            #endif     
            push(constant);                              
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
            printf("%d\n",pop());
        break;
      }
            case OP_SET: {
                int num = pop();
                int varIndex = getVar((char)pop());
                setVar(varIndex, num);
                break;
            } 
            
            case OP_GET:{
                int varIndex = getVar((char)pop());
                if(defineCheck(varIndex))
                    push(getvarValue(varIndex));
                else{
                    runtimeErrorCall();
                }
                break;
            }  

            default:
            #ifdef DEBUGER
                printf("totally toast %d", instruction);  
            #endif   
            break;       
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
    printf("syntax error\n");                        
    return INTERPRET_COMPILE_ERROR;            
  }                                            

  vm.chunk = &chunk;                           
  vm.ip = vm.chunk->code;  
    #ifdef DEBUGER
    printf("running\n");
    #endif
    InterpretResult result = run();   
    #ifdef DEBUGER           
    printf("done\n");
    #endif
  //freeChunk(&chunk);                           
  return result;                              
}  