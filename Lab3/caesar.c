#include <string.h>
#include <stdlib.h>

static void encrypt(char* text, int shift) {
    int len = strlen(text);

    for (int i = 0; i < len; i++) {
        text[i] = ((text[i] - 32 + shift) % 95) + 32;
    }
}

static void decrypt(char *text, int shift) {
    int len = strlen(text);

    for (int i = 0; i < len; i++) {
        text[i] = ((text[i] - 32 - shift + 95) % 95) + 32;
    }
}