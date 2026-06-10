#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "config.h"
#include "ledstate.h"
#include "eeprom_raw.h"

bool eeprom_save_ledstate(const ledstate *ls);
bool eeprom_load_ledstate(ledstate *ls);
