#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tokenizer.h"
#include "value.h"
#include "parser.h"


static Value *parse_many_values(Parser *parser);


static Token expect_token(char *cursor, TokenType expected) {
    Token token = scan_token(cursor);

    if (token.type != expected)
        // TODO: proper error messages
        exit(1);

    return token;
}

static bool is_number(Span span) {
    char *end;
    strtod(span.start, &end);
    return span_end(span) == end;
}

static double span_to_double(Span span) {
    return strtod(span.start, NULL);
}

static Value *parse_atom(Span atom) {
    return is_number(atom)                ? make_number(span_to_double(atom), atom)
         : span_equals_string(atom, "#t") ? make_boolean(true, atom)
         : span_equals_string(atom, "#f") ? make_boolean(false, atom)
         :                                  make_symbol(atom);
}

static Value *parse_list(Span left_paren) {
    Value *list = parse_many_values(span_end(left_paren));
    Span right_paren = expect_token(value_end(list), TOKEN_RIGHT_PAREN).span;

    // there is no separate list node, the cons chain itself is the list,
    // so we set its span to cover the parentheses
    list->span = span_cover(left_paren, right_paren);

    return list;
}

static Value *parse_value(char *cursor) {
    Token token = scan_token(cursor);

    switch (token.type) {
        case TOKEN_ATOM:        return parse_atom(token.span);
        case TOKEN_LEFT_PAREN:  return parse_list(token.span);
        default:                return NULL;
    }
}

static Value *parse_many_values(char *cursor) {
    Value *car = parse_value(cursor);

    if (car == NULL)
        return nil(cursor);

    return cons(car, parse_many_values(value_end(car)));
}

Value *parse_program(char *cursor) {
    Value *program = parse_many_values(cursor);

    expect_token(value_end(program), TOKEN_EOF);

    return program;
}
