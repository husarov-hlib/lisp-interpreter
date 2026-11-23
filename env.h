#ifndef ENV_H
#define ENV_H

#include "span.h"
#include "value.h"

typedef struct Binding {
    Span symbol;
    Value *value;
    struct Binding *next;
} Binding;

typedef struct Env {
    Binding *bindings;
    struct Env *parent;
} Env;

Env *make_env(Env *parent);
Value *env_lookup(Env *env, Span symbol);
void env_define(Env *env, Span symbol, Value *value);
void env_define_many(Env *env, Value *symbols, Value *values);

#endif
