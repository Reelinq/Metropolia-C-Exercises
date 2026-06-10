#include "ledstate.h"

void set_led_state(ledstate *ls, uint8_t value) {
    ls->state = value;
    ls->not_state = ~value;
}

bool is_led_state_valid(const ledstate *ls) {
    return ls->state == (uint8_t) ~ls->not_state;
}
