#include <stdio.h>    
#include <string.h> 
#include <stdbool.h>  
  
#include "scanner.h"


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



 char peek(){
    // printf("%c \n", *scanner.current);
    return *scanner.current;
}

 char peekNext(){
    if(isAtEnd()) return '\0';
   // printf("N: %c \n", scanner.current[1]);
    return scanner.current[1];
}

 bool match(char expected){
    if(isAtEnd()) return false;
    printf("current: %c\n", *scanner.current);
    if(*scanner.current != expected) return false;

    scanner.current++;
    return true;
}
bool matchNext(char expected){
    if(peekNext() == '\0') return false;
    if(peekNext() != expected) return false;

    //scanner.current++;
    return true;
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
    printf("scanning Token: %c \n",c);
    if(isAlpha(c)) {printf("alphatoken #%d\n", TOKEN_CHAR); return makeToken(TOKEN_CHAR);}
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
   
  }   

  printf("scanning is fucked\n");

    return errorToken("Unexpected character.");
}
