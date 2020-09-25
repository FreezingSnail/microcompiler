#include <stdio.h>    
#include <string.h> 
#include <stdbool.h>  
  
#include "scanner.h"
#include "common.h"


typedef struct{
    const char * start;
    const char * current;
} Scanner;

Scanner scanner;

void initScanner(const char * source){
    scanner.start = source;
    scanner.current = source;

}

 bool isAlpha(char c) {     
    return (c >= 'a' && c <= 'c');            
}    

 bool isDigit(char c) { 
    return c >= '0' && c <= '9';
} 

static bool isAtEnd(){
    return *scanner.current == '\0';
}

static char advance(){
    scanner.current++;
    return scanner.current[-1];
}



void next(){
    if(!isAtEnd()){
    advance();
    }
}


static Token makeToken(TokenType type){
    Token token;
    token.type = type;
    token.start = scanner.start;
    return token;
}

static Token errorToken(const char * message){
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    return token;
}

Token scanToken() {
    scanner.start = scanner.current;

    if(isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();
    #ifdef DEBUGGER
        printf("scanning Token: %c \n",c);
        printf("scanning Token: %d \n",c);
    #endif
    
    

    if(isAlpha(c)) return makeToken(TOKEN_CHAR);
    if(isDigit(c)) return makeToken(TOKEN_DIGIT);

    switch (c) {                                    

        case ';': return makeToken(TOKEN_SEMICOLON);        
        case '.': return makeToken(TOKEN_DOT);        
        case '-': return makeToken(TOKEN_MINUS);      
        case '+': return makeToken(TOKEN_PLUS);       
        case '/': return makeToken(TOKEN_SLASH);      
        case '*': return makeToken(TOKEN_STAR);

        case '!':                                                        
            return makeToken( TOKEN_BANG);  
        case '=': 
             return makeToken( TOKEN_EQUAL); 
        case '\0': 
        case 10:
            return makeToken(TOKEN_EOF);                                                
   
  }   
   
    return errorToken("Unexpected character.");
}
