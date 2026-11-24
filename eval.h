#ifndef EVAL_H
#define EVAL_H

#include "value.h"

Value *eval(Value *expr, Env *env);
void eval_program(Value *program);

#endif
