#include "eeprom.h"

bool eeprom_save_ledstate(const ledstate *ls) {
    uint8_t buf[LEDSTATE_SIZE] = { ls->state, ls->not_state };
    bool ok = eeprom_write(LEDSTATE_ADDR, buf, LEDSTATE_SIZE);
    if (ok)
        printf("Saved state=0x%02X not_state=0x%02X at addr 0x%04X to EEPROM\n",
               ls->state, ls->not_state, LEDSTATE_ADDR);
    else
        printf("EEPROM write failed!\n");
    return ok;
}

bool eeprom_load_ledstate(ledstate *ls) {
    uint8_t buf[LEDSTATE_SIZE];
    bool ok = eeprom_read(LEDSTATE_ADDR, buf, LEDSTATE_SIZE);
    if (!ok) {
        printf("EEPROM read failed!\n");
        return false;
    }
    ls->state = buf[0];
    ls->not_state = buf[1];
    printf("Read state=0x%02X not_state=0x%02X from addr 0x%04X from EEPROM\n",
           ls->state, ls->not_state, LEDSTATE_ADDR);
    return is_led_state_valid(ls);
}
