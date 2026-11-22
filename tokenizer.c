#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tokenizer.h"


char DIGITS[] = "0123456789";
char SYMBOL_CHARS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*/<>=!?_$%&:~^.";


Token *token_if_matched(TokenType type, char *start, char *end) {
    if (end == NULL) return NULL;

    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->span = span_between(start, end);

    return token;
}


char *token_end(Token *token) {
    return token ? span_end(token->span) : NULL;
}


bool is_one_of(char c, char *charset) {
    return strchr(charset, c) != NULL;
}


char *match_char(char *cursor, char expected) {
    if (cursor == NULL) return NULL;

    return (*cursor == expected) ? cursor + 1 : NULL;
}

char *match_one_of(char *cursor, char *charset) {
    if (cursor == NULL) return NULL;

    return is_one_of(*cursor, charset) ? cursor + 1 : NULL;
}

char *match_one_or_more(char *cursor, char *charset) {
    if (cursor == NULL) return NULL;

    char *end = cursor;
    while (is_one_of(*end, charset))
        end++;

    return cursor == end ? NULL : end;
}


Token *scan_left_paren(char *start) {
    return token_if_matched(TOKEN_LEFT_PAREN, start, match_char(start, '('));
}

Token *scan_right_paren(char *start) {
    return token_if_matched(TOKEN_RIGHT_PAREN, start, match_char(start, ')'));
}

Token *scan_number(char *start) {
    char *integer_end = match_one_or_more(start, DIGITS);
    char *decimal_end = match_one_or_more(match_char(integer_end, '.'), DIGITS);
    
    // if matching the decimal returns NULL, ?: will fall back to the integer match
    return token_if_matched(TOKEN_NUMBER, start, decimal_end ?: integer_end);
}

Token *scan_boolean(char *start) {
    char *end = match_one_of(match_char(start, '#'), "tf");

    return token_if_matched(TOKEN_BOOLEAN, start, end);
}

Token *scan_symbol(char *start) {
    return token_if_matched(TOKEN_SYMBOL, start, match_one_or_more(start, SYMBOL_CHARS));
}


Token *scan_token(char *cursor) {
    return scan_left_paren(cursor)
        ?: scan_right_paren(cursor)
        ?: scan_number(cursor)
        ?: scan_boolean(cursor)
        ?: scan_symbol(cursor);
}
