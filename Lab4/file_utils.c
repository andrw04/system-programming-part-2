#include <stdio.h>
#include <string.h>
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


static void write_file(const char* filename, char* text, const char* mode) {
    FILE* file = fopen(filename, mode);

    if (!file) {
        exit(1);
    }

    fprintf(file, "%s", text);
    fclose(file);
}

static void write_start_message(const char* filename, pid_t pid) {
    FILE* file = fopen(filename, "w");

    if (!file) {
        exit(1);
    }

    fprintf(file, "Daemon started on Process ID: {%d}. Waiting for signals...\n", pid);
    fclose(file);
}

static int extract_pid(const char* filename) {
    char* line = read_file(filename);

    const char *start = strstr(line, "{");
    if (start != NULL) {
        int pid;
        if (sscanf(start, "{%d}", &pid) == 1) {
            free(line);
            return pid;
        }
    }
    free(line);

    return -1; // PID не найден или неверный формат
}