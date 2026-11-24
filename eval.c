#include "builtin.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "value.h"


static bool is_symbol_named(Value *value, char *name) {
    return value->type == VALUE_SYMBOL && span_equals_string(value->span, name);
}

static void expect_operands(Value *expr, int expected) {
    int count = length(cdr(expr));

    if (count != expected)
        error(expr->span, "expected %d operands, got %d", expected, count);
}


static Value *eval_symbol(Value *expr, Env *env) {
    Value *value = env_lookup(env, expr->span);

    if (value == NULL)
        error(expr->span, "symbol not defined");

    return value;
}

static Value *eval_define(Value *expr, Env *env) {
    expect_operands(expr, 2);

    Value *symbol = second(expr);

    if (symbol->type != VALUE_SYMBOL)
        error(symbol->span, "can't define a non-symbol");

    Value *value = eval(third(expr), env);
    env_define(env, symbol->span, value);

    return value;
}

static Value *eval_lambda(Value *expr, Env *env) {
    expect_operands(expr, 2);

    return make_closure(env, second(expr), third(expr));
}

static Value *eval_if(Value *expr, Env *env) {
    expect_operands(expr, 3);

    Value *predicate = eval(second(expr), env);

    if (predicate->type == VALUE_BOOLEAN && predicate->boolean == false)
        return eval(fourth(expr), env);
    else
        return eval(third(expr), env);
}

static Value *eval_each(Value *expr, Env *env) {
    if (is_nil(expr))
        return nil(NULL);

    Value *first = eval(car(expr), env);
    Value *rest = eval_each(cdr(expr), env);

    return cons(first, rest);
}

static Value *eval_call(Value *expr, Env *env) {
    Value *function = eval(car(expr), env);
    Value *arguments = eval_each(cdr(expr), env);

    if (function->type == VALUE_BUILTIN)
        return function->builtin(arguments, expr);

    if (function->type == VALUE_CLOSURE) {
        Value *parameters = function->closure.parameters;

        expect_operands(expr, length(parameters));

        Env *call_env = make_env(function->closure.env);
        env_define_many(call_env, parameters, arguments);

        return eval(function->closure.body, call_env);
    }

    error(expr->span, "not a valid function");
}

static Value *eval_form(Value *expr, Env *env) {
    Value *operator = car(expr);

    return is_symbol_named(operator, "define") ? eval_define(expr, env)
         : is_symbol_named(operator, "lambda") ? eval_lambda(expr, env)
         : is_symbol_named(operator, "if")     ? eval_if(expr, env)
         :                                       eval_call(expr, env);
}

Value *eval(Value *expr, Env *env) {
    switch (expr->type) {
        case VALUE_SYMBOL:  return eval_symbol(expr, env);
        case VALUE_CONS:    return eval_form(expr, env);
        default:            return expr;
    }
}

void eval_program(Value *program) {
    Env *env = make_env(NULL);
    define_builtins(env);

    for (Value *expr = program; !is_nil(expr); expr = cdr(expr))
        eval(car(expr), env);
}
