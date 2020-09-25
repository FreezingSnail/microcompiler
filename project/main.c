#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "scanner.h"
#include "compiler.h"
#include "common.h"                     


static void repl(){
	char line[1024];
	for(;;){
		printf("> ");

		if (!fgets(line, sizeof(line), stdin)){
			printf("\n");
			break;
		}

		if(line[0] == ':' && line[1] == 'q')
			break;

        else if(line[0] == ':' && line[1] == 'r')
			reset();
		else	
            interpret(line); 
	}
}

static char * readFile(const char * path){
	FILE * file = fopen(path, "rb");
	if(file == NULL){
		fprintf(stderr, "Could not open file \"%s\". \n", path);
		exit(74);
	}

	fseek(file, 0l, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char * buffer = (char*)malloc(fileSize + 1);
	if(buffer == NULL){
		fprintf(stderr, "Not enough memory to read \"%s\". \n", path);
		exit(74);
	}
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if(bytesRead < fileSize){
		fprintf(stderr, "Could not read file \"%s\". \n", path);
		exit(74);
	}
	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

static void runFile(const char* path){

	char * source = readFile(path);
	interpret(source);
}

int main(int argc, char ** argv){
	initVM();

    
	if(argc ==1){
		repl();
	} else if (argc == 2){
		runFile(argv[1]);
	} else {
		fprintf(stderr, "failure\n");
		exit(64);
	}
    
	freeVM();

    


	return 0;
}
