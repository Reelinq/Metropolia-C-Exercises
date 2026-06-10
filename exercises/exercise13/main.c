#include <stdio.h>
#include <string.h>

int main() {
	char filename[100];
	printf("Enter a filename: ");
	fgets(filename, 100, stdin);

	if (filename[strlen(filename) - 1] == '\n') {
		filename[strlen(filename) - 1] = '\0';
	}

	FILE *file = fopen(filename, "r" );
	char line[100];
	int lineCount = 0;
	int min = 2147483647;
	int max = -2147483648;

	if (file == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		return 1;
	}

	while(fgets(line, 100, file) != NULL) {
		int num;
		if (sscanf(line, "%d", &num) != 1) {
			break;
		}

		lineCount++;
		printf("%2d: %d\n", lineCount, num);

		if (num < min) min = num;
		if (num > max) max = num;
	}

	printf("Count of numbers: %d\n", lineCount);
	printf("Lowest number: %d\n", min);
	printf("Highest number: %d\n", max);

	fclose(file);
}
