#include <stdio.h>                                     

#include "debug.h"    
#include "value.h"      

static int simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);                                     
  return offset + 1;                                        
}  
static int constantInstruction(const char* name, Chunk* chunk,
                               int offset) {                  
  uint8_t constant = chunk->code[offset + 1];                 
  printf("%-16s %4d '", name, constant);                      
  printValue(chunk->constants.values[constant]);              
  printf("'\n"); 
  return offset + 2;                                         
}                                                             

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);                          

  for (int offset = 0; offset < chunk->count;) {       
    offset = disassembleInstruction(chunk, offset);    
  }                                                    
}      


int disassembleInstruction(Chunk* chunk, int offset) {
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];          
  switch (instruction) {   
      case OP_CONSTANT:                                          
      return constantInstruction("OP_CONSTANT", chunk, offset);   
      case OP_VAR:    
      return constantInstruction("OP_VAR", chunk, offset);   
      case OP_ADD:                                             
      return simpleInstruction("OP_ADD", offset);              
    case OP_SUBTRACT:                                          
      return simpleInstruction("OP_SUBTRACT", offset);         
    case OP_MULTIPLY:                                          
      return simpleInstruction("OP_MULTIPLY", offset);         
    case OP_DIVIDE:                                            
      return simpleInstruction("OP_DIVIDE", offset);    
      case OP_DOT:                                          
      return simpleInstruction("OP_DOT", offset);
      case OP_PRINT:                                          
      return simpleInstruction("OP_PRINT", offset); 
      case OP_SET:                                          
      return simpleInstruction("OP_SET", offset);     
      case OP_GET:                                          
      return simpleInstruction("OP_GET", offset);                 
     
    default:                                          
      printf("Unknown opcode %d\n", instruction);     
      return offset + 1;                              
  }                                                   
}  