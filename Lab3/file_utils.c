#include <stdio.h>
#include <stdlib.h>

static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

static void write_file(const char* filename, char* text) {
    FILE* file = fopen(filename, "w");

    if (!file) {
        fprintf(stderr, "Error writing file\n");
        exit(1);
    }

    fprintf(file, "%s", text);
    fclose(file);
}