#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"


typedef enum { 
	OP_CONSTANT,
    OP_VAR,
    OP_DOT,

	OP_POP,
	OP_SET,
	OP_GET,
    

	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,

	OP_PRINT,

} OpCode; 

typedef struct {
	int count;
	int capacity;
	uint8_t  code[526];
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);
int addConstant(Chunk * chunk, int value);

#endif