#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "debug.h"

int main() {
	srand(time(NULL));

	printf("Enter debug level (0-4): ");
	int level;
	scanf("%d", &level);
	set_debug_level(level);

	for (int i = 1; i <= 5; i++) {
		int randLevel = rand() % 5;
		dprintf(randLevel, "Debug message %d)\n", i);
	}
}
