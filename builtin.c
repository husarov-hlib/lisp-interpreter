#include <stdio.h>

#include "builtin.h"
#include "env.h"
#include "error.h"
#include "value.h"


static Value *expect_operands(Value *expr, int expected) {
    Value *operands = cdr(expr);
    int count = length(operands);

    if (count == expected)
        return operands;

    error(expr->span, "expected %d operands, got %d", expected, count);
}

static double expect_number(Value *argument, Value *operand) {
    if (argument->type == VALUE_NUMBER)
        return argument->number;

    error(operand->span, "expected a number");
}

static Value *expect_cons(Value *argument, Value *operand) {
    if (argument->type == VALUE_CONS)
        return argument;

    error(operand->span, "expected a cons");
}


#define BUILTIN_ARITHMETIC(name, operator)                                  \
    static Value *builtin_##name(Value *arguments, Value *call_expr) {      \
        Value *operands = expect_operands(call_expr, 2);                    \
                                                                            \
        double result = expect_number(first(arguments), first(operands))    \
               operator expect_number(second(arguments), second(operands)); \
                                                                            \
        return make_number(result, NO_SPAN);                                \
    }

BUILTIN_ARITHMETIC(add, +);
BUILTIN_ARITHMETIC(subtract, -);
BUILTIN_ARITHMETIC(multiply, *);
BUILTIN_ARITHMETIC(divide, /);


#define BUILTIN_COMPARISON(name, operator)                                \
    static Value *builtin_##name(Value *arguments, Value *call_expr) {    \
        Value *operands = expect_operands(call_expr, 2);                  \
                                                                          \
        bool result = expect_number(first(arguments), first(operands))    \
             operator expect_number(second(arguments), second(operands)); \
                                                                          \
        return make_boolean(result, NO_SPAN);                             \
    }

BUILTIN_COMPARISON(equal, ==);
BUILTIN_COMPARISON(less, <);
BUILTIN_COMPARISON(greater, >);
BUILTIN_COMPARISON(less_or_equal, <=);
BUILTIN_COMPARISON(greater_or_equal, >=);


#define BUILTIN_TYPE_PREDICATE(name, value_type)                            \
    static Value *builtin_##name(Value *arguments, Value *call_expr) {      \
        expect_operands(call_expr, 1);                                      \
                                                                            \
        return make_boolean(first(arguments)->type == value_type, NO_SPAN); \
    }

BUILTIN_TYPE_PREDICATE(is_number, VALUE_NUMBER);
BUILTIN_TYPE_PREDICATE(is_boolean, VALUE_BOOLEAN);
BUILTIN_TYPE_PREDICATE(is_symbol, VALUE_SYMBOL);
BUILTIN_TYPE_PREDICATE(is_pair, VALUE_CONS);
BUILTIN_TYPE_PREDICATE(is_null, VALUE_NIL);

static Value *builtin_is_procedure(Value *arguments, Value *call_expr) {
    expect_operands(call_expr, 1);

    bool is_procedure = first(arguments)->type == VALUE_CLOSURE
                     || first(arguments)->type == VALUE_BUILTIN;

    return make_boolean(is_procedure, NO_SPAN);
}


static Value *builtin_display(Value *arguments, Value *call_expr) {
    expect_operands(call_expr, 1);
    print_value(first(arguments));

    return nil(NO_SPAN);
}

static Value *builtin_newline(Value *arguments, Value *call_expr) {
    (void) arguments;

    expect_operands(call_expr, 0);
    printf("\n");

    return nil(NO_SPAN);
}

static Value *builtin_car(Value *arguments, Value *call_expr) {
    Value *operands = expect_operands(call_expr, 1);

    return car(expect_cons(first(arguments), first(operands)));
}

static Value *builtin_cdr(Value *arguments, Value *call_expr) {
    Value *operands = expect_operands(call_expr, 1);

    return cdr(expect_cons(first(arguments), first(operands)));
}


static void define_builtin(Env *env, char *name, Builtin *builtin) {
    env_define(env, make_symbol(span_from_string(name)), make_builtin(builtin));
}

void define_builtins(Env *env) {
    define_builtin(env, "+", builtin_add);
    define_builtin(env, "-", builtin_subtract);
    define_builtin(env, "*", builtin_multiply);
    define_builtin(env, "/", builtin_divide);

    define_builtin(env, "=", builtin_equal);
    define_builtin(env, "<", builtin_less);
    define_builtin(env, ">", builtin_greater);
    define_builtin(env, "<=", builtin_less_or_equal);
    define_builtin(env, ">=", builtin_greater_or_equal);

    define_builtin(env, "number?", builtin_is_number);
    define_builtin(env, "boolean?", builtin_is_boolean);
    define_builtin(env, "symbol?", builtin_is_symbol);
    define_builtin(env, "pair?", builtin_is_pair);
    define_builtin(env, "null?", builtin_is_null);
    define_builtin(env, "procedure?", builtin_is_procedure);

    define_builtin(env, "display", builtin_display);
    define_builtin(env, "newline", builtin_newline);
    define_builtin(env, "car", builtin_car);
    define_builtin(env, "cdr", builtin_cdr);
}
