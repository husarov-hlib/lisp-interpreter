#include <stdbool.h>
#include <stdlib.h>

#include "parser.h"
#include "tokenizer.h"
#include "value.h"


typedef struct {
    char *cursor;
} Parser;


static Value *parse_many_values(Parser *parser);


static Token peek_token(Parser *parser) {
    return scan_token(parser->cursor);
}

static Token next_token(Parser *parser) {
    Token token = scan_token(parser->cursor);
    parser->cursor = span_end(token.span);

    return token;
}

static Token expect_token(Parser *parser, TokenType expected) {
    Token token = next_token(parser);

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


static Value *parse_atom(Parser *parser) {
    Span atom = expect_token(parser, TOKEN_ATOM).span;

    return is_number(atom)                ? make_number(span_to_double(atom), atom)
         : span_equals_string(atom, "#t") ? make_boolean(true, atom)
         : span_equals_string(atom, "#f") ? make_boolean(false, atom)
                                          : make_symbol(atom);
}

static Value *parse_list(Parser *parser) {
    Span left_paren = expect_token(parser, TOKEN_LEFT_PAREN).span;
    Value *list = parse_many_values(parser);
    Span right_paren = expect_token(parser, TOKEN_RIGHT_PAREN).span;

    // there is no separate list node, the cons chain itself is the list,
    // so we set its span to cover the parentheses
    list->span = span_cover(left_paren, right_paren);

    return list;
}

static Value *parse_value(Parser *parser) {
    Token token = peek_token(parser);

    switch (token.type) {
        case TOKEN_ATOM:        return parse_atom(parser);
        case TOKEN_LEFT_PAREN:  return parse_list(parser);
        default:                return NULL;
    }
}

static Value *parse_many_values(Parser *parser) {
    Value *car = parse_value(parser);

    if (car == NULL)
        return nil(parser->cursor);

    return cons(car, parse_many_values(parser));
}

Value *parse_program(char *source) {
    Parser parser = {.cursor = source};

    Value *program = parse_many_values(&parser);
    expect_token(&parser, TOKEN_EOF);

    return program;
}
