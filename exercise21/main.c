#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	printf("Enter a file name: ");
	char filename[100];
	scanf("%s", filename);

	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		return 1;
	}

	char line[100];
	int lineNum = 0;

	while (fgets(line, sizeof(line), file)) {
		lineNum++;

		char *dollar = strchr(line, '$');
		char *star = strchr(line, '*');

		char checksum = 0;
		for (char *p = dollar + 1; p < star; p++) {
			checksum ^= *p;
		}

		int expected;
		sscanf(star + 1, "%2x", &expected);

		line[strcspn(line, "\n")] = '\0';

		if (checksum == expected) {
			printf("[ OK ] %s\n", line);
		} else {
			printf("[FAIL] %s\n", line);
		}
	}
	fclose(file);
}
