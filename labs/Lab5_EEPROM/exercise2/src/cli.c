#include "cli.h"

void log_handle_command(void) {
    static char buf[COMMAND_BUFFER_SIZE];
    static int i = 0;
    int c;

    while ((c = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) { // Non-blocking read
        if (c == '\n' || c == '\r') {
            buf[i] = '\0';
            i = 0;
            if (buf[0] == '\0') return;

            if (strcmp(buf, "read") == 0) {
                log_read_all();
            } else if (strcmp(buf, "erase") == 0) {
                log_erase_all();
            }
            buf[0] = '\0';
            return;
        }
        if (i < COMMAND_BUFFER_SIZE - 1) buf[i++] = (char)c;
    }
}
