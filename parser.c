#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tokenizer.h"
#include "value.h"
#include "parser.h"


Value *number_if_matched(Token *token) {
    if (token == NULL) return NULL;

    char *end;
    double number = strtod(token->span.start, &end);
    assert(end - token->span.start == token->span.length);

    return make_number(number, token->span);
}

Value *boolean_if_matched(Token *token) {
    if (token == NULL) return NULL;

    return make_boolean(token->span.start[1] == 't', token->span);
}

Value *symbol_if_matched(Token *token) {
    if (token == NULL) return NULL;

    return make_symbol(token->span);
}


Token *match_token(char *cursor, TokenType expected) {
    Token *token = scan_token(cursor);

    return (token && token->type == expected) ? token : NULL;
}


Value *parse_number(char *cursor) {
    return number_if_matched(match_token(cursor, TOKEN_NUMBER));
}

Value *parse_boolean(char *cursor) {
    return boolean_if_matched(match_token(cursor, TOKEN_BOOLEAN));
}

Value *parse_symbol(char *cursor) {
    return boolean_if_matched(match_token(cursor, TOKEN_BOOLEAN));
}

Value *parse_list(char *cursor) {
    Token *left_paren = match_token(cursor, TOKEN_LEFT_PAREN);

    if (left_paren == NULL)
        return NULL;

    cursor = span_end(left_paren->span);
    Value *items = parse_many_values(cursor);

    cursor = span_end(items->span);
    Token *right_paren = match_token(cursor, TOKEN_RIGHT_PAREN);

    if (right_paren == NULL)
        return NULL;

    items->span = span_join(left_paren->span, right_paren->span);

    return items;
}

Value *parse_value(char *cursor) {
    return parse_number(cursor)
        ?: parse_boolean(cursor)
        ?: parse_symbol(cursor)
        ?: parse_list(cursor);
}

Value *parse_many_values(char *cursor) {
    Value *car = parse_value(cursor);

    if (car == NULL)
        return nil(cursor);

    return cons(car, parse_many_values(cursor));
}
