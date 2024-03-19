#include "utils.c"
#include "merge_sort.c"
#include <time.h>
#include <string.h>

#define MAX_THREAD_COUNT 8


int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Using: %s <array_length> <thread_count> [<-v/--verbose>]\n", argv[0]);
		exit(1);
	}

	if (!is_number(argv[1])) {
		fprintf(stderr, "Array length is not a number\n");
		exit(1);
	}

	if (!is_number(argv[2])) {
		fprintf(stderr, "Thread count is not a number\n");
		exit(1);
	}

	int array_length = atoi(argv[1]);
	int thread_count = atoi(argv[2]);
	int* array = generate_array(array_length, 0, array_length);

	if (thread_count > MAX_THREAD_COUNT || thread_count < 0) {
		fprintf(stderr, "Thread count must be betwen 0 and %d\n", MAX_THREAD_COUNT);
		exit(1);
	}

	if (argc >= 4 && (!strcmp(argv[3], "-v") || !strcmp(argv[3], "--verbose"))) {
        printf("Original array:\n");
		print_array(array, array_length);
	}

	clock_t start = clock();
    parallel_sort(array, array_length, thread_count);
	clock_t end = clock();

	if (argc >= 4 && (!strcmp(argv[3], "-v") || !strcmp(argv[3], "--verbose"))) {
        printf("Sorted array:\n");
		print_array(array, array_length);
	}

	double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Array length: %d | Thread count: %d | Execution time: %f\n", array_length, thread_count, execution_time);

	free(array);

	exit(0);
}