#include <stdio.h>
#include <string.h>

int studentLoop(int studentAmount, char grades[][4]);
int gradeLoop(int studentNumber, char grades[][4]);
int printGrades(int studentAmount, char grades[][4]);

int main() {
	int studentAmount;
	printf("How many students: ");
	scanf("%d", &studentAmount);

	char grades[studentAmount][4];
	for (int i = 0; i < studentAmount; i++) {
		strcpy(grades[i], "N/A");
	}

	studentLoop(studentAmount, grades);
	printGrades(studentAmount, grades);
}

int studentLoop(int studentAmount, char grades[][4]) {
	while (1) {
		int studentNumber;
		printf("\nEnter student number (1 - %d) or 0 to stop: ", studentAmount);
		scanf("%d", &studentNumber);

		if (studentNumber == 0) {
			break;
		}

		if (studentNumber > studentAmount) {
			printf("Invalid student number!\n");
		} else {
			gradeLoop(studentNumber, grades);
		}
	}
}

int gradeLoop(int studentNumber, char grades[][4]) {
	while (1) {
		int grade;
		printf("Enter grade (0 - 5) for student %d or -1 to cancel: ", studentNumber);
		scanf("%d", &grade);

		if (grade == -1) {
			break;
		}

		if (grade >= 0 && grade <= 5) {
			sprintf(grades[studentNumber - 1], "%d", grade);
			break;
		}

		printf("Invalid grade!\n");
	}
}

int printGrades(int studentAmount, char grades[][4]) {
	printf("\n%-10s %-5s\n", "Student", "Grade");
	for (int i = 0; i < studentAmount; i++) {
		printf("%-10d %-5s\n", i + 1, grades[i]);
	}
}
