#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "config.h"
#include "eeprom_raw.h"

void log_erase_all(void);
bool log_write(const char *str);
void log_read_all(void);
