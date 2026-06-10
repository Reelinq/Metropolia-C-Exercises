#include <stdio.h>

int read_range(int low, int high);

int main () {
	printf("Let's play!\n");

	int guess;
	for (int i = 0; i < 3; i++) {
		printf("Roll a die and enter your result.\n");

		printf("Enter a number between 1 and 6: ");
		guess = read_range(1, 6);

		if (guess == 6) {
			printf("I got 6. It is a tie!\n");
		} else {
			printf("I got %d. I win!\n", guess + 1);
		}
	}
	printf("Better luck next time. Bye!\n");
}

int read_range(int low, int high) {
	int number;
	scanf("%d", &number);

	if (number < low || number > high) {
		printf("invalid input\n");
	} else {
		return number;
	}
}
