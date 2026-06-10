#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "config.h"
#include "motor.h"
#include "cmd.h"

static bool calibrated = false;
static uint16_t steps_per_rot = 0;

void handle_line(char *line);

int main() {
    stdio_init_all();

    gpio_init(OPTO_FORK_PIN);
    gpio_set_dir(OPTO_FORK_PIN, GPIO_IN);
    gpio_pull_up(OPTO_FORK_PIN);

    init_motor();

    printf("Stepper motor controller ready.\n");
    printf("Commands: status | calib | run [N]\n\n");

    char buf[64];
    while (true) {
        printf("> ");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            handle_line(buf);
        }
    }
}

void handle_line(char *line) {
    line[strcspn(line, "\r\n")] = '\0';
    if (strlen(line) == 0) return;

    int n = 0;
    if (strcmp(line, "status") == 0) {
        cmd_status(calibrated, steps_per_rot);
    } else if (strcmp(line, "calib") == 0) {
        cmd_calib(&calibrated, &steps_per_rot);
    } else if (strcmp(line, "run") == 0) {
        cmd_run(8, calibrated, steps_per_rot);
    } else if (sscanf(line, "run %d", &n) == 1) {
        cmd_run(n, calibrated, steps_per_rot);
    } else {
        printf("Unknown command: '%s'. Available: status, calib, run [N]\n", line);
    }
}
