#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "tokenizer.h"


static const char WHITESPACE[] = " \t\r\n";
static const char DELIMITERS[] = " \t\r\n()";


static char *skip_whitespace(char *cursor) {
    return cursor + strspn(cursor, WHITESPACE);
}

static Token scan_atom(char *cursor) {
    Span atom = {
        .start = cursor,
        .length = strcspn(cursor, DELIMITERS)
    };

    assert(atom.length > 0);

    return (Token) {TOKEN_ATOM, atom};
}

Token scan_token(char *cursor) {
    cursor = skip_whitespace(cursor);

    switch (*cursor) {
        case '\0': return (Token) {TOKEN_EOF,         (Span) {cursor, 0}};
        case '(':  return (Token) {TOKEN_LEFT_PAREN,  (Span) {cursor, 1}};
        case ')':  return (Token) {TOKEN_RIGHT_PAREN, (Span) {cursor, 1}};
        default:   return scan_atom(cursor);
    }
}
