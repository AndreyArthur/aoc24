#include "io.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char *io_readfile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open \"%s\".\n", filename);
        exit(1);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Failed to seek \"%s\".\n", filename);
        fclose(file);
        exit(1);
        return NULL;
    }

    ptrdiff_t filesize = ftell(file);
    if (filesize == (ptrdiff_t)-1) {
        fprintf(stderr, "Failed to get size of \"%s\".\n", filename);
        fclose(file);
        exit(1);
        return NULL;
    }

    char *buf = malloc(sizeof(char) * (filesize + 1));

    rewind(file);
    size_t bytes_read = fread(buf, 1, filesize, file);
    if (bytes_read != (size_t)filesize) {
        fprintf(stderr, "Failed to read \"%s\".\n", filename);
        fclose(file);
        exit(1);
        return NULL;
    }
    buf[bytes_read] = '\0';

    fclose(file);

    return buf;
}
