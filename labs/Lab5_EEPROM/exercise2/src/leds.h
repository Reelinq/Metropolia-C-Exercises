#pragma once

#include <stdint.h>
#include <stdio.h>
#include "config.h"
#include "pico/stdlib.h"

void apply_leds(uint8_t state);
void print_state(uint8_t state);
