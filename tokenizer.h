#ifndef TOKENIZER_H
#define TOKENIZER_H


typedef enum {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_NUMBER,
    TOKEN_BOOLEAN,
    TOKEN_SYMBOL,
} TokenType;

typedef struct {
    TokenType type;
    char *start;
    int length;
} Token;

Token *next_token(char *cursor);


#endif
