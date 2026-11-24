#include "builtin.h"
#include "env.h"
#include "error.h"
#include "value.h"


static Span argument_span(Value *call_expr, int index) {
    Value *unevaluated_arguments = cdr(call_expr);

    return nth(unevaluated_arguments, index)->span;
}

static void expect_operands(Value *call_expr, int expected) {
    int count = length(cdr(call_expr));

    if (count != expected)
        error(call_expr->span, "expected %d operands, got %d", expected, count);
}

static double expect_number(Value *arguments, Value *call_expr, int index) {
    Value *argument = nth(arguments, index);

    if (argument->type != VALUE_NUMBER)
        error(argument_span(call_expr, index), "expected a number");

    return argument->number;
}


#define BUILTIN_ARITHMETIC(name, operator)                             \
    static Value *builtin_##name(Value *arguments, Value *call_expr) { \
        expect_operands(call_expr, 2);                                 \
                                                                       \
        double result = expect_number(arguments, call_expr, 0)         \
               operator expect_number(arguments, call_expr, 1);        \
                                                                       \
        return make_number(result, NO_SPAN);                           \
    }

BUILTIN_ARITHMETIC(add, +);
BUILTIN_ARITHMETIC(subtract, -);
BUILTIN_ARITHMETIC(multiply, *);
BUILTIN_ARITHMETIC(divide, /);


static void define_builtin(Env *env, char *name, Builtin *builtin) {
    env_define(env, span_from_string(name), make_builtin(builtin));
}

void define_builtins(Env *env) {
    define_builtin(env, "+", builtin_add);
    define_builtin(env, "-", builtin_subtract);
    define_builtin(env, "*", builtin_multiply);
    define_builtin(env, "/", builtin_divide);
}
