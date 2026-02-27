#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

static int global_debug_level = 0;

void set_debug_level(int debug_level) {
	global_debug_level = debug_level;
}

int dprintf(int debug_level, const char *fmt, ...) {
	if (debug_level > global_debug_level) {
		return 0;
	}

	fprintf(stderr, "[DBG%d] ", debug_level);

	va_list args;
	va_start(args, fmt);
	int result = vfprintf(stderr, fmt, args);
	va_end(args);

	return result;
}
