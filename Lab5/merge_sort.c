#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct {
    int *ptr;
    int length;
} ArrayPart;


static int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

static void sort(void* array_part) {
    ArrayPart *part = (ArrayPart*)array_part;
    qsort(part->ptr, part->length, sizeof(int), compare);
    pthread_exit(NULL);
}


static void merge(ArrayPart parts[], int thread_count) {
    int* left = parts[0].ptr;
    int leftLength = parts[0].length;

    for (int p = 1; p < thread_count; p++) {
        int* right = parts[p].ptr;
        int rightLength = parts[p].length;

        int* buffer = (int*)malloc((leftLength + rightLength) * sizeof(int));

        int left_index = 0, right_index = 0, buffer_index = 0;
        while (left_index < leftLength && right_index < rightLength) {
            if (left[left_index] <= right[right_index]) {
                buffer[buffer_index] = left[left_index++];
            } else {
                buffer[buffer_index] = right[right_index++];
            }
            buffer_index++;
        }

        while (left_index < leftLength) {
            buffer[buffer_index++] = left[left_index++];
        }

        while (right_index < rightLength) {
            buffer[buffer_index++] = right[right_index++];
        }

        leftLength += rightLength;

        int *array = left;
        for (int i = 0; i < leftLength; i++) {
            array[i] = buffer[i];
        }

        free(buffer);
    }
}


static void parallel_sort(int* array, int array_length, int thread_count) {
    pthread_t threads[thread_count];
    ArrayPart parts[thread_count];

    int part_size = array_length / thread_count;

    for (int i = 0; i < thread_count; i++) {
        parts[i].ptr = array + i * part_size;
        parts[i].length = (i == thread_count - 1) ? (array_length - i * part_size) : part_size;
        pthread_create(&threads[i], NULL, (void *(*)(void *)) sort, &parts[i]);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    merge(parts, thread_count);
}