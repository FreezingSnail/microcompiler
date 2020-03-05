all: first
first: main.c 
	gcc vm.c main.c scanner.c compiler.c chunk.c debug.c memory.c value.c -o main &&./main
clean:
	rm -rf first
