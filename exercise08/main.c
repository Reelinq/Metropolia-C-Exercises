#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_numbers(const int *array, int count);

int main() {
	int numbers[15];
	int count = 15;
	srand(time(NULL));

	for (int i = 0; i < count; i++) {
		numbers[i] = rand();
	}

	print_numbers(numbers, count);

	return 0;
}

void print_numbers(const int *array, int count) {
	for (int i = 0; i < count; i++) {
		printf("%8d\n", array[i]);
	}
}
