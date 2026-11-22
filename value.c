#include <stdlib.h>

#include "value.h"


Value *make_value(ValueType type, Span span) {
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
    return make_value(VALUE_BOOLEAN, span);
}

Value *cons(Value *car, Value *cdr) {
    Value *value = make_value(VALUE_CONS, span_join(car->span, cdr->span));
    value->cons.car = car;
    value->cons.cdr = cdr;

    return value;
}

Value *nil(char *cursor) {
    return make_value(VALUE_NIL, span_between(cursor, cursor));
}


char *value_end(Value *value) {
    return span_end(value->span);
}
