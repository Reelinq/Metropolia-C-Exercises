#include <stdio.h>

int replace_char(char *str, const char *repl);

int main() {
	char str[100];

	printf("Enter a string: ");
	fgets(str, 100, stdin);

	char replace[100];

	printf("Enter character to replace and replacement character (example: ae): ");
	fgets(replace, 100, stdin);

	int count = replace_char(str, replace);

	if (count == 0) {
		printf("String was not modified\n");
		return 0;
	}
	printf("Number of replacements: %d\n", count);
	printf("Modified string: %s", str);
}

int replace_char(char *str, const char *repl) {
	if (repl[2] != '\0') {
		return 0;
	}

	int count = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == repl[0]) {
			str[i] = repl[1];
			count++;
		}
	}
	return count;
}
