#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "eval.h"
#include "parser.h"


static char *read_file(char *path) {
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *text = malloc(size + 1);
    fread(text, 1, size, file);
    text[size] = '\0';

    fclose(file);

    return text;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    source = read_file(argv[1]);

    eval_program(parse_program(source));

    return EXIT_SUCCESS;
}
