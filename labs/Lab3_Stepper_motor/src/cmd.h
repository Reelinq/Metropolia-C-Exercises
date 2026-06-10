#pragma once

#include <stdbool.h>
#include <stdint.h>

void cmd_status(bool calibrated, uint16_t steps_per_rot);
void cmd_calib(bool *calibrated, uint16_t *steps_per_rot);
void cmd_run(int n, bool calibrated, uint16_t steps_per_rot);
