#include <stdlib.h>

#include "value.h"


Value *make_value(ValueType type) {
    Value *value = malloc(sizeof(Value));
    value->type = type;

    return value;
}

Value *make_number(double number, Span span) {
    Value *value = make_value(VALUE_NUMBER);
    value->number = number;
    value->span = span;

    return value;
}

Value *make_boolean(bool boolean, Span span) {
    Value *value = make_value(VALUE_BOOLEAN);
    value->boolean = boolean;
    value->span = span;

    return value;
}

Value *make_symbol(Span span) {
    Value *value = make_value(VALUE_BOOLEAN);
    value->span = span;

    return value;
}

Value *cons(Value *car, Value *cdr) {
    Value *value = make_value(VALUE_CONS);
    value->cons.car = car;
    value->cons.cdr = cdr;
    value->span = span_join(car->span, cdr->span);

    return value;
}

Value *nil() {
    return make_value(VALUE_NIL);
}
