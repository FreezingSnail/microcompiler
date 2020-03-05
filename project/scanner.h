#ifndef clox_scanner_h               
#define clox_scanner_h   
#include <stdbool.h>  


typedef enum {                                        
  // Single-character tokens.                         
  TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,    
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

  // One or two character tokens.                     
  TOKEN_BANG, TOKEN_EQUAL,                      

  // Literals.                                        
  TOKEN_CHAR, TOKEN_DIGIT,       

              

  TOKEN_ERROR,                                        
  TOKEN_EOF                                           
} TokenType;   

typedef struct{
    TokenType type;
    const char * start;
} Token;


char peek();
char peekNext();
bool isTerminal();
bool match(char expected);
bool isAlpha(char c);
bool isDigit(char c);

void next();

void initScanner(const char* source);
Token scanToken();


#endif  