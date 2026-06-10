#include <stdio.h>
#include <stdint.h>

#include "cmd.h"
#include "calib.h"
#include "motor.h"

void cmd_status(bool calibrated, uint16_t steps_per_rot) {
    printf("Calibrated: %s\n", calibrated ? "Yes" : "No");
    if (calibrated)
        printf("Steps per revolution: %u\n", steps_per_rot);
    else
        printf("Steps per revolution: not available\n");
}

void cmd_calib(bool *calibrated, uint16_t *steps_per_rot) {
    printf("Starting calibration...\n");
    uint16_t counts[3];
    for (uint8_t i = 0; i < 3; i++) {
        printf("Revolution %d/3...\n", i + 1);
        counts[i] = measure_one_rotation();
        if (counts[i] == 0) {
            printf("Calibration failed.\n");
            motor_off();
            return;
        }
        printf("Steps counted: %u\n", counts[i]);
    }
    align_opto_fork();
    *steps_per_rot = (counts[0] + counts[1] + counts[2]) / 3;
    *calibrated = true;
    printf("Calibration done. Steps per revolution: %u\n", *steps_per_rot);
}

void cmd_run(int n, bool calibrated, uint16_t steps_per_rot) {
    if (!calibrated) {
        printf("Error: motor not calibrated. Run 'calib' first.\n");
        return;
    }

    uint16_t steps;
    if (n == 8) {
        steps = steps_per_rot;
        printf("Running one full rotation (%u steps)...\n", steps);
    } else if (n <= 0 || n > 8) {
        printf("Error: invalid argument '%d'. Use 'run' or 'run N' where N is 1..7.\n", n);
        return;
    } else {
        steps = (uint16_t)(((uint32_t)steps_per_rot * (uint32_t)n) / 8);
        printf("Running %d/8 of a rotation (%u steps)...\n", n, steps);
    }

    for (uint16_t i = 0; i < steps; i++) {
        motor_step();
    }
    motor_off();
    printf("Done.\n");
}
