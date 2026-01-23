#include <stdio.h>
#include <stdlib.h>

int validate_input(int low, int high);
int print_number(int range);

int main() {
	printf("Hello, select an operation.\n");
	int operation;

	while (operation != 0) {
		printf("1 - Roll D6\n");
		printf("2 - Roll D10\n");
		printf("0 - Exit\n");

		operation = validate_input(0, 2);

		if (operation == 1) {
			printf("\nYou selected 'Roll D6'.\n");
			print_number(6);
		} else if (operation == 2) {
			printf("\nYou selected 'Roll D10'.\n");
			print_number(10);
		}
	}
}

int validate_input(int low, int high) {
	int number;
	scanf("%d", &number);

	if (number < low || number > high) {
		printf("\nInvalid selection. Please try again.\n");
		return 3;
	} else {
		return number;
	}
}

int print_number(int range) {
	printf("%d\n", (rand() % range) + 1);
}
