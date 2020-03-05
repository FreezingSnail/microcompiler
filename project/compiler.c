#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "compiler.h"
#include "debug.h"



/*
    TODO:
    error handling










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

/*
Input: 
   string ω 
   parsing table M for grammar G

Output:
   If ω is in L(G) then left-most derivation of ω,
   error otherwise.

Initial State : $S on stack (with S being start symbol)
   ω$ in the input buffer

SET ip to point the first symbol of ω$.

repeat
   let X be the top stack symbol and a the symbol pointed by ip.

   if X∈ Vt or $
      if X = a
         POP X and advance ip.
      else
         error()
      endif
      
   else	// X is non-terminal 
      if M[X,a] = X → Y1, Y2,... Yk    
         POP X
         PUSH Yk, Yk-1,... Y1 // Y1 on top 
         Output the production X → Y1, Y2,... Yk 
      else
         error()
      endif
   endif
until X = $	// empty stack
*/

typedef struct {       
  Token current;       
  Token previous;      
} Parser;

Parser parser; 

Chunk* compilingChunk;

static Chunk* currentChunk() {                          
  return compilingChunk;                                
}  

int vara = 0;
int varb = 0;
int varc = 0;

char stack[256];
char * stackpointer = stack;
int stackSize = 0;

static void StmtList();

static void advance() {             
    printf("advancine\n");
  

  parser.previous = parser.current;

  //for (;;) {                                      
    parser.current = scanToken();                 
    //if (parser.current.type != TOKEN_ERROR) //break;
    //printf("id p %c\n",*parser.previous.start);
    //printf("id c %c\n",*parser.current.start);
    //printf("id p %d\n",parser.previous);
   // printf("id c %d\n",parser.current);
    //errorAtCurrent(parser.current.start);         
  //}                                               
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
  disassembleChunk(currentChunk(), "code");    
}                          


static void consume(TokenType type, const char* message) {
  if (parser.current.type == type) {                      
    advance();                                            
    return;                                               
  }

  //errorAtCurrent(message);                                
}   


bool compile(const char* source, Chunk* chunk){
  initScanner(source);    

  compilingChunk = chunk;          
    printf("first advance\n");
  advance();                                      
  Prog();                                   
  consume(TOKEN_EOF, "Expect end of expression."); 
  printf("done compiling\n");
  endCompiler();
  return true;

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


static void resolve(){
    if(isAlpha(stack[0])){
    char c = stack[0];
    switch(c){
        case 'a': printf("%i\n",vara); break;
            case 'b': printf("%i\n",varb); break;
            case 'c': printf("%i\n",varc); break;
    }
    }
}

static void error(char T){
    printf("error %c \n", T);
}

static void emitConstant(int value) {       
  emitBytes(OP_CONSTANT, makeConstant(value));
}   

static void Const(){
     printf("enter cosnt\n");
    printf("cost %d\n",*parser.current.start);
    int value = (int) *parser.previous.start - '0';
    emitConstant(value);
    
}

static void Id(){
     printf("enter id\n");
     printf("id p %c\n",*parser.previous.start);
    //printf("id c %c\n",*parser.current.start);
     emitBytes(OP_VAR, makeConstant(*parser.previous.start));
}

static void Expr(){
    printf("enter expr\n");
    advance();
    TokenType operatorType = parser.previous.type;

    printf("exprprev: %d\n",  operatorType);
    if(operatorType == TOKEN_CHAR){
        //advance();
        Id();
        emitByte(OP_GET);
    }
    else if( operatorType == TOKEN_DIGIT)
        Const();
    else if(operatorType == TOKEN_ERROR){
                printf("ded\n");
        return;
    }
    else {
        if(operatorType == TOKEN_EOF){
        printf("ded\n");
        return;
        }
        printf("expressioning;\n");
        //advance();
        printf("expr1\n");
        Expr();
        //advance();
        printf("expr2\n");
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

    printf("exit expr\n");
}

static void print(){

   /* printf("print ");
    char c = peekNext();
    if(isAlpha(c)){
        switch(c){
            case 'a': printf("%i\n",vara); break;
            case 'b': printf("%i\n",varb); break;
            case 'c': printf("%i\n",varc); break;
        }
    }
    else if(isDigit(c))
        printf("%c",c);
    //print("Print: %c\n", peek());
    //next();
    */
    
    advance();
    Id();
    emitByte(OP_GET);
    emitByte(OP_PRINT);
    printf("exiting print\n");
}

static void Assign(){  // god only knows
        printf("assinging\n");
        Id();
        advance();
        Expr();
        printf("pushing opset\n");
        emitByte(OP_SET);
        /*
        if(peek() == '='){
            push('=');
            next();
            Expr();
        }
        else
            error('T');
            */
            printf("exiting assign\n");
} 

static void Stmt(){
    TokenType operatorType = parser.previous.type;
    printf("stmt: %d\n",operatorType);

   if(operatorType == TOKEN_BANG){

    printf("matched !\n");
    //advance();
    print();
   } else if(parser.current.type == TOKEN_EQUAL){
        Assign();
       // advance();
   }
    else {
        printf("%d\n", parser.previous.type);
        error('S');
        print("5s\n", peek());
        //exit(70);
    }
    printf("exited Stmt\n");
}

static void nextStmt(){
     if(parser.current.type == TOKEN_SEMICOLON){ 
         advance();
         if(parser.current.type == TOKEN_DOT){ //done
        }
        else{
            
        StmtList();
        }//done
        }
}

static void StmtList(){

    printf("enteringstmtlist\n");
    advance();
    Stmt();
    nextStmt();
    if(parser.current.type == TOKEN_DOT){ //done
    advance();
    if(parser.current.type != TOKEN_EOF){
        error('f');
        }
    }
        
    //emitByte(TOKEN_SEMICOLON);
    //advance();
    
        

    
        
   /* resolve();
    if(peek() == ';')
        if(peekNext() != '.'){

            StmtList();
        }
    //else
       // error('L');*/
}



 void Prog(){
     //if(peek() != '.')
    StmtList();
    
    

}