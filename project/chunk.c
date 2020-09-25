#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"
#include "vm.h"

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	initValueArray(&chunk->constants);
}

void freeChunk(Chunk * chunk){
	initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte){
	chunk->code[chunk->count] = byte;
	chunk->count++;
}

int addConstant(Chunk* chunk, int value){
	writeValueArray(&chunk->constants, value);
	return(chunk->constants.count - 1);
}