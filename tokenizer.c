#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tokenizer.h"


Token *token_if_matched(TokenType type, char *start, char *end) {
    if (end == NULL) return NULL;

    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->start = start;
    token->length = end - start;

    return token;
}


bool is_one_of(char c, char *charset) {
    return strchr(charset, c) != NULL;
}

bool is_digit(char c) {
    return is_one_of(c, "0123456789");
}

bool is_alpha(char c) {
    return is_one_of(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

bool is_symbol_char(char c) {
    return is_alpha(c) || is_one_of(c, "+-*/<>=!?_$%&:~^.");
}


char *match_char(char *cursor, char expected) {
    if (cursor == NULL) return NULL;

    return (*cursor == expected) ? cursor + 1 : NULL;
}

char *match_one_of(char *cursor, char *charset) {
    if (cursor == NULL) return NULL;

    return is_one_of(*cursor, charset) ? cursor + 1 : NULL;
}

char *match_one_or_more(char *cursor, bool (*predicate)(char)) {
    if (cursor == NULL) return NULL;

    char *end = cursor;
    while (predicate(*end))
        end++;

    return cursor == end ? NULL : end;
}


Token *left_paren(char *start) {
    return token_if_matched(TOKEN_LEFT_PAREN, start, match_char(start, '('));
}

Token *right_paren(char *start) {
    return token_if_matched(TOKEN_RIGHT_PAREN, start, match_char(start, ')'));
}

Token *number(char *start) {
    char *integer_end = match_one_or_more(start, is_digit);
    char *decimal_end = match_one_or_more(match_char(integer_end, '.'), is_digit);
    
    // if matching the decimal returns NULL, ?: will fall back to the integer match
    return token_if_matched(TOKEN_NUMBER, start, decimal_end ?: integer_end);
}

Token *boolean(char *start) {
    char *end = match_one_of(match_char(start, '#'), "tf");

    return token_if_matched(TOKEN_BOOLEAN, start, end);
}

Token *symbol(char *start) {
    return token_if_matched(TOKEN_SYMBOL, start, match_one_or_more(start, is_symbol_char));
}


Token *next_token(char *cursor) {
    return left_paren(cursor)
        ?: right_paren(cursor)
        ?: number(cursor)
        ?: boolean(cursor)
        ?: symbol(cursor);
}
