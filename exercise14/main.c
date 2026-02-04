#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
	printf("Enter a filename: ");
	char filename[100];
	fgets(filename, 100, stdin);

	if (filename[strlen(filename) - 1] == '\n') {
		filename[strlen(filename) - 1] = '\0';
	}

	char line[80];
	int lineCount = 0;
	char outputArray[100][80];

	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		return 1;
	}

	while(fgets(outputArray[lineCount], 80, file) != NULL && lineCount < 100) {
		lineCount++;
	}
	fclose(file);

	for (int i = 0; i < lineCount; i++) {
		for (int j = 0; outputArray[i][j] != '\0'; j++) {
			outputArray[i][j] = toupper(outputArray[i][j]);
		}
	}

	file = fopen(filename, "w");

	for (int i = 0; i < lineCount; i++) {
		fputs(outputArray[i], file);
	}
	fclose(file);
}
