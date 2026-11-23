#ifndef ERROR_H
#define ERROR_H

#include "span.h"

void error(char *source, Span span, char *format, ...)
    __attribute__((format(printf, 3, 4)));

#endif
