#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "span.h"

typedef enum {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_ATOM,
    TOKEN_EOF,
} TokenType;

typedef struct {
    TokenType type;
    Span span;
} Token;

Token scan_token(char *cursor);

#endif
