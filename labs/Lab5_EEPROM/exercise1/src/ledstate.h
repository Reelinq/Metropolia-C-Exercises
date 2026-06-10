#pragma once

#include <stdint.h>
#include <stdbool.h>

// 'not_state' is always ~state. used to validate EEPROM reads.
typedef struct ledstate {
    uint8_t state;
    uint8_t not_state;
} ledstate;

void set_led_state(ledstate *ls, uint8_t value);
bool is_led_state_valid(const ledstate *ls);
