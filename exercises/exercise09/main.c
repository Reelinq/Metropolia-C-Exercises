#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int find_first(const unsigned int *array, unsigned int what);

int main() {
	unsigned int array[20];

	srand(time(NULL));

	for (unsigned int i = 0; i < 20; i++) {
		if (i == 19) {
			array[i] = 0;
		} else {
			array[i] = rand() % 20 + 1;
		}
		printf("%d\n", array[i]);
	}

	while (1) {
		int number;
		printf("Enter a number to search for (0 to stop): ");
		scanf("%d", &number);

		if (number == 0) {
			break;
		}

		if (find_first(array, number) == -1) {
			printf("Not found\n");
		} else {
			printf("The number %d was found at index %d.\n", number, find_first(array, number));
		}
	}
}

int find_first(const unsigned int *array, unsigned int what) {
	for (unsigned int i = 0; i < 20; i++) {
		if (array[i] == 0) {
			return -1;
		}

		if (array[i] == what) {
			return i;
		}
	}
}
