#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tokenizer.h"
#include "value.h"
#include "parser.h"


Token expect_token(char *cursor, TokenType expected) {
    Token token = scan_token(cursor);

    if (token.type != expected)
        // TODO: proper error messages
        exit(1);

    return token;
}

bool is_number(Span span) {
    char *end;
    strtod(span.start, &end);
    return span_end(span) == end;
}

double span_to_double(Span span) {
    return strtod(span.start, NULL);
}

Value *parse_atom(Span atom) {
    return is_number(atom)                ? make_number(span_to_double(atom), atom)
         : span_equals_string(atom, "#t") ? make_boolean(true, atom)
         : span_equals_string(atom, "#f") ? make_boolean(false, atom)
         :                                  make_symbol(atom);
}

Value *parse_list(Span left_paren) {
    Value *list = parse_many_values(span_end(left_paren));
    Span right_paren = expect_token(value_end(list), TOKEN_RIGHT_PAREN).span;

    list->span = span_join(left_paren, right_paren);

    return list;
}

Value *parse_value(char *cursor) {
    Token token = scan_token(cursor);

    switch (token.type) {
        case TOKEN_ATOM:        return parse_atom(token.span);
        case TOKEN_LEFT_PAREN:  return parse_list(token.span);
        default:                return NULL;
    }
}

Value *parse_many_values(char *cursor) {
    Value *car = parse_value(cursor);

    if (car == NULL)
        return nil(cursor);

    return cons(car, parse_many_values(value_end(car)));
}
