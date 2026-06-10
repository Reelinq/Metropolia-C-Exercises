#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "config.h"
#include "motor.h"
#include "calib.h"

bool wait_for_rising_edge(uint16_t max_steps);
bool wait_for_falling_edge(uint16_t max_steps);

static uint16_t last_slot_width = 0;
static bool at_leading_edge = false;

uint16_t measure_one_rotation(void) {
    if (!at_leading_edge) {
        if (!wait_for_falling_edge(MAX_STEPS)) {
            printf("Error: could not find first falling edge\n");
            return 0;
        }
        at_leading_edge = true;
    }

    uint16_t count = 0;
    uint16_t slot_width = 0;

    for (uint16_t i = 0; i < MAX_STEPS_SLOT; i++) {
        motor_step();
        count++;
        if (gpio_get(OPTO_FORK_PIN) == 1) {
            slot_width = count;
            break;
        }
        if (i == MAX_STEPS_SLOT - 1) {
            printf("Error: opto fork stuck low\n");
            at_leading_edge = false;
            return 0;
        }
    }
    last_slot_width = slot_width;

    for (uint16_t i = count; i < MAX_STEPS; i++) {
        motor_step();
        count++;
        if (gpio_get(OPTO_FORK_PIN) == 0) {
            at_leading_edge = true;
            return count;
        }
    }

    printf("Error: could not find second falling edge\n");
    at_leading_edge = false;
    return 0;
}

bool wait_for_falling_edge(uint16_t max_steps) {
    if (gpio_get(OPTO_FORK_PIN) == 0) {
        if (!wait_for_rising_edge(max_steps)) return false;
    }
    for (uint16_t i = 0; i < max_steps; i++) {
        motor_step();
        if (gpio_get(OPTO_FORK_PIN) == 0) return true;
    }
    return false;
}

bool wait_for_rising_edge(uint16_t max_steps) {
    for (uint16_t i = 0; i < max_steps; i++) {
        motor_step();
        if (gpio_get(OPTO_FORK_PIN) == 1) return true;
    }
    return false;
}

bool align_opto_fork(void) {
    if (!at_leading_edge) {
        if (!wait_for_falling_edge(MAX_STEPS)) {
            printf("Error: could not align opto fork\n");
            motor_off();
            return false;
        }
        at_leading_edge = true;
    }

    uint16_t half = (last_slot_width / 2);
    for (uint16_t i = 0; i < half; i++)
        motor_step();

    at_leading_edge = false;
    motor_off();
    return true;
}
