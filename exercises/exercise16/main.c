# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct node {
	int number;
	struct node *next;
} nnode;

int main() {
	nnode *head = NULL;

	while (1) {
		printf("Enter a number or 'end' to stop: ");
		char input[10];
		fgets(input, 10, stdin);

		if (input[strlen(input) - 1] == '\n') {
			input[strlen(input) - 1] = '\0';
		}

		if (strcmp(input, "end") == 0) {
			break;
		}

		char *end;
		int number = (int)strtol(input, &end, 10);
		if (*end != '\0') {
			printf("Invalid input. Please enter a number or 'end'.\n");
		} else {
			nnode *newNode = (nnode *)malloc(sizeof(nnode));
			(*newNode).number = number;
			(*newNode).next = head;
			head = newNode;
		}
	}

	nnode *current = head;
	while (current != NULL) {
		printf("%d\n", (*current).number);
		current = (*current).next;
	}

	while (head != NULL) {
		nnode *temp = head;
		head = (*head).next;
		free(temp);
	}
}
