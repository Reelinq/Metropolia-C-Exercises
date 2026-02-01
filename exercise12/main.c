#include <stdio.h>
#include <string.h>

int count_words(const char* str, const char *word);

int main() {
	char str[100];
	char word[20] = "";

	while (1) {
		printf("Enter a string: ");
		fgets(str, 100, stdin);

		if (str[strlen(str) - 1] == '\n') {
			str[strlen(str) - 1] = '\0';
		}

		printf("Enter a word to count: ");
		fgets(word, 20, stdin);

		if (word[strlen(word) - 1] == '\n') {
			word[strlen(word) - 1] = '\0';
		}

		if (strcmp(word, "stop") == 0) {
			break;
		}

		int count = count_words(str, word);
		printf("%d\n", count);
	}
}

int count_words(const char *str, const char *word) {
	int count = 0;

	while ((str = strstr(str, word))) {
		count++;
		str += strlen(word);
	}

	return count;
}
