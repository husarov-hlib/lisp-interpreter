#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "span.h"

typedef enum {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_NUMBER,
    TOKEN_BOOLEAN,
    TOKEN_SYMBOL,
} TokenType;

typedef struct {
    TokenType type;
    Span span;
} Token;

char *token_end(Token *token);

Token *scan_token(char *cursor);

#endif
