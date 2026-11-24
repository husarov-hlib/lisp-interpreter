#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

#include "span.h"

typedef struct Env Env;
typedef struct Value Value;

typedef Value *Builtin(Value *arguments, Value *call, char *source);

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
Value *make_closure(Env *env, Value *parameters, Value *body);
Value *make_builtin(Builtin *builtin);
Value *cons(Value *car, Value *cdr);
Value *nil(char *cursor);

Value *car(Value *list);
Value *cdr(Value *list);
Value *second(Value *list);
Value *third(Value *list);
Value *fourth(Value *list);
Value *nth(Value *list, int index);
bool is_nil(Value *value);
int length(Value *list);
void print_value(Value *value);

#endif
