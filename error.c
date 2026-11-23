#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "span.h"


static int count_lines(char *source, char *cursor) {
    int lines = 1;

    for (char *c = source; c < cursor; c++)
        if (*c == '\n')
            lines++;

    return lines;
}

static char *find_line_start(char *source, char *cursor) {
    while (cursor > source && cursor[-1] != '\n')
        cursor--;

    return cursor;
}

static void print_repeated(char c, int count) {
    for (int i = 0; i < count; i++)
        putc(c, stderr);
}

void error(char *source, Span span, char *format, ...) {
    va_list arguments;

    va_start(arguments, format);
    fprintf(stderr, "error: ");
    vfprintf(stderr, format, arguments);
    putc('\n', stderr);
    va_end(arguments);

    int line_number = count_lines(source, span.start);
    char *line_start = find_line_start(source, span.start);
    int line_length = strcspn(line_start, "\n");

    fprintf(stderr, "%3d | %.*s\n", line_number, line_length, line_start);

    fprintf(stderr, "    | ");
    print_repeated(' ', span.start - line_start);
    print_repeated('^', span.length);
    putc('\n', stderr);

    exit(EXIT_FAILURE);
}
