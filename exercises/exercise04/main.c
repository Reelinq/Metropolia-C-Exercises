#include <stdio.h>

int read_integer(void);

int main() {
	int count = 0;
	int sum = 0;
	int number;

	while (1) {
		number = read_integer();

		if (number < 0) {
			break;
		}

		sum += number;
		count ++;
	}

	float average = (float)sum / count;
	printf("\nYou entered %d positive numbers. The average is: %.3f\n", count, average);
}

int read_integer(void) {
	int number;

	while (1) {
		printf("\nEnter positive numbers or negative to stop: ");

		if (scanf("%d", &number) == 1) {
			return number;
		} else {
			printf("invalid input\n");
		}
	}
}
