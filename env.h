#ifndef ENV_H
#define ENV_H

#include "value.h"

typedef struct Binding {
    Value *symbol;
    Value *value;
    struct Binding *next;
} Binding;

typedef struct Env {
    Binding *bindings;
    struct Env *parent;
} Env;

Env *make_env(Env *parent);
Value *env_lookup(Env *env, Value *symbol);
void env_define(Env *env, Value *symbol, Value *value);
void env_define_many(Env *env, Value *symbols, Value *values);

#endif
