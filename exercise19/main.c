#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct menu_item_ {
	char name[50];
	double price;
} menu_item;

int compareByPrice(const void *a, const void *b);
int compareByName(const void *a, const void *b);

int main() {
	printf("Enter a file name: ");
	char filename[100];
	fgets(filename, 100, stdin);

	if (filename[strlen(filename) - 1] == '\n') {
		filename[strlen(filename) - 1] = '\0';
	}

	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		return 1;
	}

	int lineCount = 0;
	menu_item outputArray[40];

	char line[100];
	while (fgets(line, 100, file) != NULL && lineCount < 40) {
		char *semicolon = strchr(line, ';');
		if (semicolon) {
			*semicolon = '\0';
			strcpy(outputArray[lineCount].name, line);
			sscanf(semicolon + 1, "%lf", &outputArray[lineCount].price);
			lineCount++;
		}
	}
	fclose(file);

	printf("Choose sorting order (0 for price, 1 for name): ");
	int sortChoice;
	scanf("%d", &sortChoice);

	if (sortChoice == 0) {
		qsort(outputArray, lineCount, sizeof(menu_item), compareByPrice);
	} else if (sortChoice == 1) {
		qsort(outputArray, lineCount, sizeof(menu_item), compareByName);
	}

	printf("%8s  %s\n", "Price", "Name");
	for (int i = 0; i < lineCount; i++) {
		printf("%8.2f  %s\n", outputArray[i].price, outputArray[i].name);
	}
}

int compareByPrice(const void *a, const void *b) {
	double diff = ((const menu_item *)a)->price - ((const menu_item *)b)->price;
	return (diff > 0) - (diff < 0);
}

int compareByName(const void *a, const void *b) {
	return strcmp(((const menu_item *)a)->name, ((const menu_item *)b)->name);
}
