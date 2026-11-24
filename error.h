#ifndef ERROR_H
#define ERROR_H

#include <stdnoreturn.h>

#include "span.h"

extern char *source;

noreturn void error(Span span, char *format, ...)
    __attribute__((format(printf, 2, 3)));

#endif
