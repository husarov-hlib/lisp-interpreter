#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

#include "span.h"

typedef struct Env Env;
typedef struct Value Value;

typedef Value *Builtin(Value *arguments);

typedef enum {
    VALUE_NUMBER,
    VALUE_BOOLEAN,
    VALUE_SYMBOL,
    VALUE_CLOSURE,
    VALUE_BUILTIN,
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
            Env *env;
            Value *parameters;
            Value *body;
        } closure;
        Builtin *builtin;
        struct {
            Value *car;
            Value *cdr;
        } cons;
    };
} Value;

Value *make_number(double number, Span span);
Value *make_boolean(bool boolean, Span span);
Value *make_symbol(Span span);
Value *cons(Value *car, Value *cdr);
Value *nil(char *cursor);

Value *car(Value *expr);
Value *cdr(Value *expr);
Value *second(Value *expr);
Value *third(Value *expr);
bool is_nil(Value *value);
int length(Value *value);

#endif
