#pragma once

#include "config.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hardware/i2c.h"

bool eeprom_write(uint16_t mem_addr, const uint8_t *data, size_t len);
bool eeprom_read(uint16_t mem_addr, uint8_t *data, size_t len);
