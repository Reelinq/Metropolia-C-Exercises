#include <stdio.h>
#include <stdbool.h>

bool read_positive(int *value);

int main () {
	int number;
	int incorrect_counter = 0;
	while (incorrect_counter < 3) {
		printf("Guess how much money I have!\n");
		printf("Enter a positive number: ");

		if (read_positive(&number)) {
			printf("You didn't get it right. I have %d euros.\n", number);
		} else {
			printf("Incorrect input\n");
			incorrect_counter++;
		}
	}
	printf("I give up! See you later!\n");
}

bool read_positive(int *value) {
	int number;
	int result = scanf("%d", &number);

	if (result != 1 || number < 0) {
		while ((getchar()) != '\n');
		return false;
	} else {
		*value = number * 2 + 20;
		while ((getchar()) != '\n');
		return true;
	}
}
