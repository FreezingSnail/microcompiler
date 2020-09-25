#include <stdio.h>                      

#include "memory.h"                     
#include "value.h"                      

void initValueArray(ValueArray* array) {
               
  array->capacity = 0;                  
  array->count = 0;                     
}     

void writeValueArray(ValueArray* array, int value) {       

  array->values[array->count] = value;                       
  array->count++;                                            
}   


void freeValueArray(ValueArray* array) {            
  initValueArray(array);                            
}  

void printValue(int value) {
  printf("%d", value);        
}  