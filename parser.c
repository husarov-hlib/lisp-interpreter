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

Value *list_if_matched(Token *left_paren, Value *items, Token *right_paren) {
    if (left_paren == NULL || items == NULL || right_paren == NULL) return NULL;

    // list is just its items (which are themselves just a cons-chain),
    // but a list's span includes the opening and closing parentheses
    items->span = span_join(left_paren->span, right_paren->span);

    return items;
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
    Value *items = parse_many_values(token_end(left_paren));
    Token *right_paren = match_token(value_end(items), TOKEN_RIGHT_PAREN);

    return list_if_matched(left_paren, items, right_paren);
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

    return cons(car, parse_many_values(value_end(car)));
}
