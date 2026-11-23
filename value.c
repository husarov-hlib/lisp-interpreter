#include <assert.h>
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

Value *cons(Value *car, Value *cdr) {
    Value *value = make_value(VALUE_CONS, span_cover(car->span, cdr->span));
    value->cons.car = car;
    value->cons.cdr = cdr;

    return value;
}

Value *nil(char *cursor) {
    return make_value(VALUE_NIL, span_between(cursor, cursor));
}


Value *car(Value *expr) {
    assert(expr->type == VALUE_CONS);
    return expr->cons.car;
}

Value *cdr(Value *expr) {
    assert(expr->type == VALUE_CONS);
    return expr->cons.cdr;
}

Value *second(Value *expr) {
    return car(cdr(expr));
}

Value *third(Value *expr) {
    return car(cdr(cdr(expr)));
}

bool is_nil(Value *value) {
    return value->type == VALUE_NIL;
}

int length(Value *value) {
    if (is_nil(value))
        return 0;

    return 1 + length(cdr(value));
}
