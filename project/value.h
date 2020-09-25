#ifndef value_h 
#define value_h 

#include "common.h"  



typedef struct {     
  int capacity;      
  int count;         
  int values[256];     
} ValueArray;  

void initValueArray(ValueArray* array);              
void writeValueArray(ValueArray* array, int value);
void freeValueArray(ValueArray* array);    
//for debugger
void printValue(int value);          



#endif   