#ifndef SPAN_H
#define SPAN_H

#include <string.h>
#include <stdbool.h>

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

static inline Span span_cover(Span first, Span last) {
    return span_between(first.start, span_end(last));
}

static inline bool span_equals_string(Span span, char *string) {
    return span.length == strlen(string)
        && strncmp(span.start, string, span.length) == 0;
}

#endif
