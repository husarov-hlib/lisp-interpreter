#ifndef ERROR_H
#define ERROR_H

#include <stdnoreturn.h>

#include "span.h"

noreturn void error(char *source, Span span, char *format, ...)
    __attribute__((format(printf, 3, 4)));

#endif
