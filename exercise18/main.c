#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));
	int number;

	while (1) {
		printf("Enter a number between 0 and 15 (negative to stop): ");
		scanf("%d", &number);

		if (number < 0) {
			break;
		}

		if (number <= 15) {
			int randomNumber = rand();
			printf("Random number: %x\n", randomNumber);

			int shifted = randomNumber >> number;
			int result = shifted & 0x3F;
			printf("Result: %02x\n", result);
		}
	}
}
