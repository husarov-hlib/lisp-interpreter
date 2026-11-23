#include "env.h"
#include "error.h"
#include "eval.h"
#include "value.h"


static bool is_symbol_named(Value *value, char *name) {
    return value->type == VALUE_SYMBOL && span_equals_string(value->span, name);
}

static void expect_operands(Value *expr, int expected, Context context) {
    int operands = length(cdr(expr));

    if (operands != expected)
        error(context.source, expr->span, "expected %d operands, got %d", expected, operands);
}


static Value *eval_symbol(Value *expr, Context context) {
    Value *value = env_lookup(context.env, expr->span);

    if (value == NULL)
        error(context.source, expr->span, "symbol not defined");

    return value;
}

static Value *eval_define(Value *expr, Context context) {
    expect_operands(expr, 2, context);

    Value *symbol = second(expr);

    if (symbol->type != VALUE_SYMBOL)
        error(context.source, symbol->span, "can't define a non-symbol");

    Value *value = eval(third(expr), context);
    env_define(context.env, symbol->span, value);

    return value;
}

static Value *eval_if(Value *expr, Context context) {
    expect_operands(expr, 3, context);

    Value *predicate = eval(second(expr), context);

    if (predicate->type == VALUE_BOOLEAN && predicate->boolean == false)
        return eval(fourth(expr), context);
    else
        return eval(third(expr), context);
}

static Value *eval_each(Value *expr, Context context) {
    if (is_nil(expr))
        return nil(NULL);

    Value *first = eval(car(expr), context);
    Value *rest = eval_each(cdr(expr), context);

    return cons(first, rest);
}

static Value *eval_call(Value *expr, Context context) {
    Value *function = eval(car(expr), context);
    Value *arguments = eval_each(cdr(expr), context);

    if (function->type == VALUE_BUILTIN)
        return function->builtin(arguments);

    if (function->type == VALUE_CLOSURE) {
        Context call_context = {
            .env = make_env(function->closure.env),
            .source = context.source
        };

        env_define_many(call_context.env, function->closure.parameters, arguments);

        return eval(function->closure.body, call_context);
    }

    error(context.source, expr->span, "not a valid function");
}

static Value *eval_form(Value *expr, Context context) {
    Value *operator = car(expr);

    return is_symbol_named(operator, "define") ? eval_define(expr, context)
         : is_symbol_named(operator, "if")     ? eval_if(expr, context)
         :                                       eval_call(expr, context);
}

Value *eval(Value *expr, Context context) {
    switch (expr->type) {
        case VALUE_SYMBOL:  return eval_symbol(expr, context);
        case VALUE_CONS:    return eval_form(expr, context);
        default:            return expr;
    }
}

void eval_program(Value *program, char *source) {
    Context context = {
        .env = make_env(NULL),
        .source = source
    };

    for (Value *expr = program; !is_nil(expr); expr = cdr(expr))
        eval(car(expr), context);
}
