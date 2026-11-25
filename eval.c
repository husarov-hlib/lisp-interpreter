#include "builtin.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "value.h"


static bool is_symbol_named(Value *value, char *name) {
    return value->type == VALUE_SYMBOL && span_equals_string(value->span, name);
}

static Value *expect_operands(Value *expr, int expected) {
    Value *operands = cdr(expr);
    int count = length(operands);

    if (count == expected)
        return operands;

    error(expr->span, "expected %d operands, got %d", expected, count);
}

static Value *expect_symbol(Value *expr) {
    if (expr->type == VALUE_SYMBOL)
        return expr;

    error(expr->span, "expected a symbol");
}

static Value *expect_list(Value *expr) {
    if (is_nil(expr) || is_cons(expr))
        return expr;

    error(expr->span, "expected a list");
}


static Value *eval_symbol(Value *expr, Env *env) {
    Value *value = env_lookup(env, expr);

    if (value == NULL)
        error(expr->span, "symbol not defined");

    return value;
}

static Value *eval_define(Value *expr, Env *env) {
    Value *operands = expect_operands(expr, 2);

    Value *symbol = expect_symbol(first(operands));
    Value *value = eval(second(operands), env);
    env_define(env, symbol, value);

    return value;
}

static Value *eval_lambda(Value *expr, Env *env) {
    Value *operands = expect_operands(expr, 2);

    Value *parameters = expect_list(first(operands));
    Value *body = second(operands);

    for (Value *rest = parameters; !is_nil(rest); rest = cdr(rest))
        expect_symbol(car(rest));

    return make_closure(env, parameters, body);
}

static Value *eval_if(Value *expr, Env *env) {
    Value *operands = expect_operands(expr, 3);

    Value *test = eval(first(operands), env);

    return is_truthy(test) ? eval(second(operands), env)
                           : eval(third(operands), env);
}

static Value *eval_quote(Value *expr) {
    Value *operands = expect_operands(expr, 1);

    return first(operands);
}

static Value *eval_each(Value *list, Env *env) {
    if (is_nil(list))
        return nil(NO_SPAN);

    Value *value = eval(car(list), env);
    Value *rest = eval_each(cdr(list), env);

    return cons(value, rest, NO_SPAN);
}

static Value *apply_closure(Closure closure, Value *arguments, Value *call_expr) {
    expect_operands(call_expr, length(closure.parameters));

    Env *env = make_env(closure.env);
    env_define_many(env, closure.parameters, arguments);

    return eval(closure.body, env);
}

static Value *eval_call(Value *expr, Env *env) {
    Value *function = eval(car(expr), env);
    Value *arguments = eval_each(cdr(expr), env);

    switch (function->type) {
        case VALUE_BUILTIN: return function->builtin(arguments, expr);
        case VALUE_CLOSURE: return apply_closure(function->closure, arguments, expr);
        default:            error(car(expr)->span, "not a valid function");
    }
}

static Value *eval_form(Value *expr, Env *env) {
    Value *operator = car(expr);

    return is_symbol_named(operator, "define") ? eval_define(expr, env)
         : is_symbol_named(operator, "lambda") ? eval_lambda(expr, env)
         : is_symbol_named(operator, "if")     ? eval_if(expr, env)
         : is_symbol_named(operator, "quote")  ? eval_quote(expr)
                                               : eval_call(expr, env);
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

    for (Value *rest = program; !is_nil(rest); rest = cdr(rest))
        eval(car(rest), env);
}
