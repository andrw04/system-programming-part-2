#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>


static int* generate_array(int length, int min, int max) {
    int* array = (int*)malloc(length * sizeof(int));

    if (!array) {
        exit(1);
    }

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        array[i] = min + rand() % (max - min + 1);
    }

    return array;
}


static void print_array(int* array, int array_length) {
    for (int i = 0; i < array_length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}


static int is_number(const char* str) {
    int len = strlen(str);

    int i = 0;

    if (*str == '-')
        i++;

    while (i < len) {
        if (!isdigit(*(str + i++)))
            return 0;
    }

    return 1;
}

