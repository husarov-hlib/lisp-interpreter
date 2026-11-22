#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

#include "span.h"

typedef enum {
    VALUE_NUMBER,
    VALUE_BOOLEAN,
    VALUE_SYMBOL,
    VALUE_CONS,
    VALUE_NIL,
} ValueType;

typedef struct Value {
    ValueType type;
    Span span;
    union {
        double number;
        bool boolean;
        struct {
            struct Value *car;
            struct Value *cdr;
        } cons;
    };
} Value;

Value *make_number(double number, Span span);
Value *make_boolean(bool boolean, Span span);
Value *make_symbol(Span span);
Value *cons(Value *car, Value *cdr);
Value *nil(char *cursor);

#endif
