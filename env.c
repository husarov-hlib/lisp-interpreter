#include <assert.h>
#include <stdlib.h>

#include "env.h"
#include "span.h"
#include "value.h"


Env *make_env(Env *parent) {
    Env *env = malloc(sizeof(Env));

    env->parent = parent;
    env->bindings = NULL;

    return env;
}

Value *env_lookup(Env *env, Span symbol) {
    for (Env *frame = env; frame != NULL; frame = frame->parent)
        for (Binding *binding = frame->bindings; binding != NULL; binding = binding->next)
            if (span_equals(binding->symbol, symbol))
                return binding->value;

    return NULL;
}

void env_define(Env *env, Span symbol, Value *value) {
    Binding *binding = malloc(sizeof(Binding));

    binding->symbol = symbol;
    binding->value = value;
    binding->next = env->bindings;

    env->bindings = binding;
}

void env_define_many(Env *env, Value *symbols, Value *values) {
    assert(length(symbols) == length(values));

    for (; !is_nil(symbols); symbols = cdr(symbols), values = cdr(values))
        env_define(env, car(symbols)->span, car(values));
}
