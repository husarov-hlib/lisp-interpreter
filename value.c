#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "value.h"


static Value *make_value(ValueType type, Span span) {
    Value *value = malloc(sizeof(Value));
    value->type = type;
    value->span = span;

    return value;
}

Value *make_number(double number, Span span) {
    Value *value = make_value(VALUE_NUMBER, span);
    value->number = number;

    return value;
}

Value *make_boolean(bool boolean, Span span) {
    Value *value = make_value(VALUE_BOOLEAN, span);
    value->boolean = boolean;

    return value;
}

Value *make_symbol(Span span) {
    return make_value(VALUE_SYMBOL, span);
}

Value *make_closure(Env *env, Value *parameters, Value *body) {
    Value *value = make_value(VALUE_CLOSURE, NO_SPAN);
    value->closure.env = env;
    value->closure.parameters = parameters;
    value->closure.body = body;

    return value;
}

Value *make_builtin(Builtin *builtin) {
    Value *value = make_value(VALUE_BUILTIN, NO_SPAN);
    value->builtin = builtin;

    return value;
}

Value *cons(Value *car, Value *cdr) {
    Value *value = make_value(VALUE_CONS, span_cover(car->span, cdr->span));
    value->cons.car = car;
    value->cons.cdr = cdr;

    return value;
}

Value *nil(char *cursor) {
    return make_value(VALUE_NIL, span_between(cursor, cursor));
}


Value *car(Value *value) {
    assert(is_cons(value));
    return value->cons.car;
}

Value *cdr(Value *value) {
    assert(is_cons(value));
    return value->cons.cdr;
}

Value *second(Value *list) {
    return car(cdr(list));
}

Value *third(Value *list) {
    return car(cdr(cdr(list)));
}

Value *fourth(Value *list) {
    return car(cdr(cdr(cdr(list))));
}

Value *nth(Value *list, int index) {
    assert(index >= 0);

    if (index == 0)
        return car(list);

    return nth(cdr(list), index-1);
}

int length(Value *list) {
    if (is_nil(list))
        return 0;

    return 1 + length(cdr(list));
}


bool is_cons(Value *value) {
    return value->type == VALUE_CONS;
}

bool is_nil(Value *value) {
    return value->type == VALUE_NIL;
}

static bool is_false(Value *value) {
    return value->type == VALUE_BOOLEAN && value->boolean == false;
}

bool is_truthy(Value *value) {
    return !is_false(value);
}


void print_value(Value *value) {
    switch (value->type) {
        case VALUE_NUMBER:
            printf("%.15g", value->number);
            break;
        case VALUE_BOOLEAN:
            printf("%s", value->boolean ? "#t" : "#f");
            break;
        case VALUE_SYMBOL:
            printf("%.*s", value->span.length, value->span.start);
            break;
        case VALUE_NIL:
            printf("nil");
            break;
        case VALUE_CONS:
            printf("(");
            for (Value *list = value; !is_nil(cdr(list)); list = cdr(list))
                print_value(car(list));
            printf(")");
            break;
        default:
            printf("unprintable");
    }
}
