#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define MAX_COURSES 10
#define MAX_STUDENTS 30
#define MAX_GRADE 5
#define MIN_GRADE 0
#define FILENAME "student_db.txt"

typedef struct {
	char course[MAX_LENGTH];
	int grade;
} Enrollment;

typedef struct {
	char name[MAX_LENGTH];
	Enrollment courses[MAX_COURSES];
	int course_count;
} Student;

int file_reader(Student students[], int *student_count);
char *trim(char *str);
void set_current_course(char *trimmed, char *current_course);
void save_to_structs(char *trimmed, Student students[], int *student_count, char *current_course, int line_number);
int find_student(Student students[], int count, const char *name);
void printer(Student students[], int student);

int main() {
	Student students[MAX_STUDENTS];
	int student_count = 0;

	if (file_reader(students, &student_count) != 0) {
		return 1;
	}

	char input[MAX_LENGTH];
	while (1) {
		printf("Enter student name (or 'stop' to quit): ");
		if (fgets(input, sizeof(input), stdin) == NULL) {
			break;
		}

		if (input[strlen(input) - 1] == '\n') {
			input[strlen(input) - 1] = '\0';
		}
		char *name = trim(input);

		if (strcmp(name, "stop") == 0) {
			break;
		}

		if (name[0] != '\0') {
			int student_index = find_student(students, student_count, name);
			if (student_index >= 0) {
				printer(students, student_index);
			} else {
				printf("Student '%s' not found.\n\n", name);
			}
		} else {
			printf("Error: Empty name entered. Please try again.\n\n");
		}
	}
}

int file_reader(Student students[], int *student_count) {
	FILE *file = fopen(FILENAME, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", FILENAME);
		return 1;
	}

	char line[MAX_LENGTH];
	char current_course[MAX_LENGTH] = "";
	int line_number = 0;

	while (fgets(line, sizeof(line), file) != NULL) {
		line_number++;

		if (strchr(line, '\n') == NULL && !feof(file)) {
			fprintf(stderr, "Warning: Line %d is too long, truncated.\n", line_number);
			int ch;
			while (ch != '\n' && ch != EOF); {
				ch = fgetc(file);
			}
		}

		line[strcspn(line, "\n")] = '\0';
		if (line[0] != '\0') {
			char *trimmed = trim(line);

			if (trimmed[0] == '[') {
				set_current_course(trimmed, current_course);
			} else if (current_course[0] != '\0') {
				if (*student_count < MAX_STUDENTS) {
					save_to_structs(trimmed, students, student_count, current_course, line_number);
				} else {
					fprintf(stderr, "Error: Maximum number of students (%d) reached, skipping line %d.\n", MAX_STUDENTS, line_number);
				}
			} else {
				fprintf(stderr, "Error: '%s' appears outside of any course, ignoring.\n", trimmed);
			}
		}
	}
	fclose(file);
	return 0;
}

char *trim(char *str) {
	while (*str == ' ' || *str == '\t') {
		str++;
	}
	if (*str == '\0') {
		return str;
	}
	char *end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t')) {
		end--;
	}
	*(end + 1) = '\0';
	return str;
}

void set_current_course(char *trimmed, char *current_course) {
	char *start = trimmed + 1;
	char *end = strchr(start, ']');
	if (end) {
		*end = '\0';
	}

	char *colon = strchr(start, ':');
	if (colon) {
		*colon = '\0';
	}

	start = trim(start);
	strncpy(current_course, start, MAX_LENGTH - 1);
	current_course[MAX_LENGTH - 1] = '\0';
}

void save_to_structs(char *trimmed, Student students[], int *student_count, char *current_course, int line_number) {
	char *colon = strchr(trimmed, ':');
	if (!colon) {
		fprintf(stderr, "Error: Line %d: '%s' has incorrect format (missing ':'), ignoring.\n", line_number, trimmed);
		return;
	}

	*colon = '\0';
	char *name = trim(trimmed); // Calling trim second time to trim spaces inside the name & grade part
	char *grade_str = trim(colon + 1);

	if (name[0] == '\0') {
		fprintf(stderr, "Error: Line %d: Empty student name, ignoring.\n", line_number);
		return;
	}

	char *endptr;
	long grade = strtol(grade_str, &endptr, 10);
	if (*endptr != '\0' || endptr == grade_str) {
		fprintf(stderr, "Error: Line %d: Invalid grade '%s', ignoring.\n", line_number, grade_str);
		return;
	}

	if (grade < MIN_GRADE || grade > MAX_GRADE) {
		fprintf(stderr, "Error: Line %d: Grade %ld out of range (%d-%d), ignoring.\n", line_number, grade, MIN_GRADE, MAX_GRADE);
		return;
	}

	int student_index = find_student(students, *student_count, name);
	if (student_index < 0) {
		if (*student_count >= MAX_STUDENTS) {
			fprintf(stderr, "Warning: Maximum number of students (%d) reached, ignoring.\n", MAX_STUDENTS);
			return;
		}
		student_index = *student_count;
		strncpy(students[student_index].name, name, MAX_LENGTH - 1);
		students[student_index].name[MAX_LENGTH - 1] = '\0';
		students[student_index].course_count = 0;
		(*student_count)++;
	}

	if (students[student_index].course_count >= MAX_COURSES) {
		fprintf(stderr, "Warning: Student '%s' has too many courses (%d max), ignoring.\n", name, MAX_COURSES);
		return;
	}

	int course_index = students[student_index].course_count;
	strncpy(students[student_index].courses[course_index].course, current_course, MAX_LENGTH - 1);
	students[student_index].courses[course_index].course[MAX_LENGTH - 1] = '\0';
	students[student_index].courses[course_index].grade = (int)grade;
	students[student_index].course_count++;
}

int find_student(Student students[], int count, const char *name) {
	for (int i = 0; i < count; i++) {
		if (strcmp(students[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void printer(Student students[], int student) {
	Student *s = &students[student];
	printf("\n%s\n", s->name);
	printf("%-*s %5s\n\n", MAX_LENGTH, "Course", "Grade");
	int total = 0;
	for (int i = 0; i < s->course_count; i++) {
		printf("%-*s %5d\n", MAX_LENGTH, s->courses[i].course, s->courses[i].grade);
		total += s->courses[i].grade;
	}
	printf("\nAverage grade: %.2f\n\n", (double)total / s->course_count);
}
