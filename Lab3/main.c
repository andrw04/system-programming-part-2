#include <stdlib.h>
#include <ctype.h>
#include "file_utils.c"
#include "caesar.c"

int is_number(const char* str) {
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        if (!isdigit(*str))
            return 0;
    }

    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Using: %s <enc/dec> <input_file> <output_file> <shift>\n", argv[0]);
        return 1;
    }

    const char* action = argv[1];
    const char* input = argv[2];
    const char* output = argv[3];

    if (!is_number(argv[4])) {
        fprintf(stderr, "Shift must be a number\n");
        return 1;
    }
    int shift = atoi(argv[4]);

    char* text = read_file(input);

    if (!strcmp(action, "enc")) {
        encrypt(text, shift);
    }
    else if (!strcmp(action, "dec")) {
        decrypt(text, shift);
    }
    else {
        fprintf(stderr, "Incorrect operation parameter. Use 'enc' or 'dec'\n");
        free(text);
        return 1;
    }

    write_file(output, text);

    free(text);

    return 0;
}