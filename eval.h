#ifndef EVAL_H
#define EVAL_H

#include "value.h"

typedef struct {
    Env *env;
} Context;

Value *eval(Value *expr, Context context);
void eval_program(Value *program);

#endif
