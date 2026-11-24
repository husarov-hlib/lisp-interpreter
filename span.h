#ifndef SPAN_H
#define SPAN_H

#include <stdbool.h>
#include <string.h>

typedef struct {
    char *start;
    int length;
} Span;

static const Span NO_SPAN = {
    .start = NULL,
    .length = 0
};

static inline Span empty_span_at(char *position) {
    return (Span) {
        .start = position,
        .length = 0
    };
}

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

static inline Span span_from_string(char *string) {
    return (Span) {
        .start = string,
        .length = (int) strlen(string)
    };
}

static inline bool span_equals(Span s1, Span s2) {
    return s1.length == s2.length
        && strncmp(s1.start, s2.start, s1.length) == 0;
}

static inline bool span_equals_string(Span span, char *string) {
    return span_equals(span, span_from_string(string));
}

#endif
