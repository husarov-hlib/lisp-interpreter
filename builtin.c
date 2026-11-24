#include "builtin.h"
#include "env.h"
#include "error.h"
#include "value.h"


static Span argument_span(Value *call, int index) {
    Value *unevaluated_arguments = cdr(call);

    return nth(unevaluated_arguments, index)->span;
}

static void expect_operands(Value *call, int expected, char *source) {
    int count = length(cdr(call));

    if (count != expected)
        error(source, call->span, "expected %d operands, got %d", expected, count);
}

static double expect_number(Value *arguments, Value *call, int index, char *source) {
    Value *argument = nth(arguments, index);

    if (argument->type != VALUE_NUMBER)
        error(source, argument_span(call, index), "expected a number");

    return argument->number;
}


#define BUILTIN_ARITHMETIC(name, operator)                                      \
    static Value *builtin_##name(Value *arguments, Value *call, char *source) { \
        expect_operands(call, 2, source);                                       \
                                                                                \
        double result = expect_number(arguments, call, 0, source)               \
               operator expect_number(arguments, call, 1, source);              \
                                                                                \
        return make_number(result, NO_SPAN);                                    \
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
