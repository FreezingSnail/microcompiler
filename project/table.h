#ifndef clox_table_h
#define clox_table_h

#include "common.h" 
#include "value.h"  

typedef struct{
    char key;
    int value;
} Entry;

typedef struct{    
  int count;        
  int capacity;     
  Entry* entries;   
} Table;    

void initTable(Table* table);



#endif   