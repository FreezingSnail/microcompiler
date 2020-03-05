#ifndef value_h 
#define value_h 

#include "common.h"  



typedef struct {     
  int capacity;      
  int count;         
  int* values;     
} ValueArray;  

void initValueArray(ValueArray* array);              
void writeValueArray(ValueArray* array, int value);
void freeValueArray(ValueArray* array);    
void printValue(int value);          



#endif   