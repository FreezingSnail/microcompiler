#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "compiler.h"




/*
    Connor Franc
    This file contains the recursive descent parser


*/
/*
hProgrami ::= hStmtListi .
hStmtListi ::= hStmti ; hStmtListi | 
hStmti ::= hAssigni | hPrinti
hAssigni ::= hIdi = hExpr i
hPrinti ::= ! hIdi
hExpr i ::= + hExpr i hExpr i
| - hExpr i hExpr i
| * hExpr i hExpr i
| / hExpr i hExpr i
| hIdi
| hConsti
hIdi ::= a | b | c
hConsti ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

*/


typedef struct {       
  Token current;       
  Token previous;      
} Parser;

Parser parser; 

Chunk* compilingChunk;
bool COMPILE_OK;
  
char stack[256];
char * stackpointer = stack;
int stackSize = 0;

static void StmtList();
static void Prog();

static void advance() {   
    #ifdef DEBUGER          
        printf("advancine\n");
    #endif
    parser.previous = parser.current;                                   
    parser.current = scanToken();                                                              
}  

static Chunk* currentChunk() {                          
  return compilingChunk;                                
}

static void emitByte(uint8_t byte) {                     
  writeChunk(currentChunk(), byte);
}  

static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);                                   
  emitByte(byte2);                                   
}  

static void emitDot() {
  emitByte(OP_DOT);    
}  

static uint8_t makeConstant(int value) {          
  int constant = addConstant(currentChunk(), value);
  if (constant > 256) {                       
    printf("Too many constants in one chunk.");      
    return 0;                                       
  }

  return (uint8_t)constant;                         
}   

static void endCompiler() {
    emitDot();      
    #ifdef DEBUGER  
        disassembleChunk(currentChunk(), "code");  
    #endif
}                          



bool compile(const char* source, Chunk* chunk){
    initScanner(source);  
    COMPILE_OK = true;  

    compilingChunk = chunk; 
    #ifdef DEBUGER         
        printf("first advance\n");
    #endif
    advance();                                      
    Prog();                                   

    #ifdef DEBUGER
        printf("done compiling\n");
    #endif
    endCompiler();
    return COMPILE_OK;

}  


static void push(char c){
    *stackpointer = c;
    stackpointer++;
    stackSize++;
}

void printStack(){
    for (int i = 0; i < stackSize; i++)
        printf("%c", stack[i]);
    printf("\n");
}

static void resetStack(){
    stackpointer = stack;
    stackSize = 0;
}

static void error(char T){
    COMPILE_OK = false;
    #ifdef DEBUGER
        printf("error %c \n", T);
    #endif
}

static void emitConstant(int value) {       
  emitBytes(OP_CONSTANT, makeConstant(value));
}   




//rdp

static void Const(){
    #ifdef DEBUGER
        printf("enter cosnt\n");
        printf("cost %d\n",*parser.current.start);
    #endif
    int value = (int) *parser.previous.start - '0';
    emitConstant(value);
    
}

static void Id(){
    #ifdef DEBUGER
        printf("enter id\n");
        printf("id p %c\n",*parser.previous.start);
    #endif
    emitBytes(OP_VAR, makeConstant(*parser.previous.start));
}

static void Expr(){
    #ifdef DEBUGER
        printf("enter expr\n");
    #endif
    advance();
    TokenType operatorType = parser.previous.type;
    #ifdef DEBUGER
        printf("exprprev: %d\n",  operatorType);
    #endif
    if(operatorType == TOKEN_CHAR){
        Id();
        emitByte(OP_GET);
    }
    else if( operatorType == TOKEN_DIGIT)
        Const();
    else if(operatorType == TOKEN_ERROR){
        error('e');
        return;
    }
    else {
        if(operatorType == TOKEN_EOF){
            return;
        }
        #ifdef DEBUGER
            printf("expressioning;\n");
            printf("expr1\n");
        #endif
        Expr();
        #ifdef DEBUGER
            printf("expr2\n");
        #endif
        Expr();
        
        switch(operatorType){
            case TOKEN_PLUS:          emitByte(OP_ADD); break;     
            case TOKEN_MINUS:         emitByte(OP_SUBTRACT); break;
            case TOKEN_STAR:          emitByte(OP_MULTIPLY); break;
            case TOKEN_SLASH:         emitByte(OP_DIVIDE); break; 

            default:
                error('E');
                return;
        }
        
    }
    #ifdef DEBUGER
        printf("exit expr\n");
    #endif
}

static void print(){
    if(parser.current.type != TOKEN_CHAR){
        error('p');
    }

    advance();
    Id();
    emitByte(OP_GET);
    emitByte(OP_PRINT);
    #ifdef DEBUGER
        printf("exiting print\n");
    #endif
}

static void Assign(){  
        #ifdef DEBUGER
            printf("assinging\n");
        #endif
        Id();
        advance();
        Expr();
        #ifdef DEBUGER
            printf("pushing opset\n");
        #endif
        emitByte(OP_SET);
        #ifdef DEBUGER
            printf("exiting assign\n");
        #endif
} 

static void Stmt(){
    TokenType operatorType = parser.previous.type;
    #ifdef DEBUGER
        printf("stmt: %d\n",operatorType);
    #endif

    if(operatorType == TOKEN_BANG){
        #ifdef DEBUGER
            printf("matched !\n");
        #endif
    print();
   } else if(parser.current.type == TOKEN_EQUAL){
       if(operatorType != TOKEN_CHAR)
            error('s');

        Assign();
   }
    else {
            #ifdef DEBUGER
                printf("%d\n", parser.previous.type);
            #endif
            error('S');
    }
    #ifdef DEBUGER
        printf("exited Stmt\n");
    #endif
}

static void nextStmt(){
     if(parser.current.type == TOKEN_SEMICOLON){ 
         #ifdef DEBUGER
            printf("semicollen nxtstmt\n");
        #endif
            advance();
            if(parser.current.type == TOKEN_DOT){ 
                #ifdef DEBUGER
                printf("done\n");//done
                #endif
            }   
            else{
                StmtList();
            }
    }
    else if(parser.current.type == TOKEN_DOT){
        #ifdef DEBUGER
            printf("checking past end of file\n");//done
            printf("token: %d\n",parser.current.type);
        #endif

        advance();

        if(parser.current.type != TOKEN_EOF)
            error('.');
    }
}

static void StmtList(){
    if(parser.current.type != TOKEN_DOT){
        #ifdef DEBUGER
            printf("enteringstmtlist\n");
        #endif
        advance();
        Stmt();
        #ifdef DEBUGER
            printf("entering nextstmtlist\n");
        #endif
        nextStmt();
    } 
     
}


 static void Prog(){
    StmtList();

     if(parser.current.type == TOKEN_DOT){ //done
        advance();
        if(parser.current.type != TOKEN_EOF){
            #ifdef DEBUGER
            printf("not eof\n");
            #endif
            error('f');
        }
     }
}