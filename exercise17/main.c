#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool pass_generator(char *password, int size, const char *word);

int main() {
	char word[32];
	char password[50];

	while (1) {
		printf("Enter a word to place inside the password (Enter 'stop' to stop programm): ");
		fgets(word, 32, stdin);

		if (word[strlen(word) - 1] == '\n') {
			word[strlen(word) - 1] = '\0';
		}

		if (strcmp(word, "stop") == 0) {
			return 0;
		}

		if (pass_generator(password, 50, word)) {
			printf("Generated password: %s\n", password);
		} else {
			printf("Password generation failed. Not enough space.\n");
		}
	}
}

bool pass_generator(char *password, int size, const char *word) {
	int pass_len = strlen(word) * 2 + 1;

	if (pass_len + 1 > size)
		return false;

	int word_len = strlen(word);
	for (int i = 0; i < word_len; i++) {
		password[i * 2] = rand() % 95 + 32;
		password[i * 2 + 1] = word[i];
		if (i == word_len - 1)
			password[i * 2 + 2] = rand() % 95 + 32;
	}

	password[pass_len] = '\0';
	return true;
}
