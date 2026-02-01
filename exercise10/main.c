#include <stdio.h>
#include <string.h>

int main() {
	char str[30];
	while (strcmp(str, "stop") != 0)
	{
		printf("\nEnter a string: ");
		fgets(str, 20, stdin);

		if (str[strlen(str) - 1] == '\n') {
			str[strlen(str) - 1] = '\0';
		}

		printf("Length of the string is: %d\n", strlen(str));
	}
	printf("\nGoodbye!\n");
}
