#include <stdlib.h>           
#include <string.h>           

#include "memory.h"           
#include "object.h"           
#include "table.h"            
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table* table){
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table* table){
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

static Entry* findEntry(Entry* entries, int capacity, char key){
    uint32_t index = key->hash % capacity;
    Entry* tombstone = NULL;

    for(;;){
        Entry* entry = &entries[index];

        if(entry->key == NULL){
            if(IS_NIL(entry->value)){
                //empty
                return tombstone != NULL ? tombstone : entry;
            } else {
                //found a tombstone
                if(tombstone == NULL) tombstone = entry;
            }
        }else if(entry->key == key){
            //the key is in the hashtable and found
            return entry;
        }
        index = (index + 1) % capacity;
    }
}