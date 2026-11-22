#ifndef SPAN_H
#define SPAN_H

#include <stddef.h>

typedef struct {
    char *start;
    int length;
} Span;

static inline char *span_end(Span span) {
    return span.start + span.length;
}

static inline Span span_between(char *start, char *end) {
    return (Span) {
        .start = start,
        .length = (int) (end - start)
    };
}

static inline Span span_join(Span first, Span last) {
    return span_between(first.start, span_end(last));
}

#endif
