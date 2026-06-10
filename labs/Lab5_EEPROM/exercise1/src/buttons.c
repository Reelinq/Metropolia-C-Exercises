#include "buttons.h"

void debounce_button(uint pin) {
    int sw_ticks = 0;
    int sw_state = 1; // last stable state (1 = released)

    while (true) {
        sleep_ms(DEBOUNCE_SLEEP_MS);
        int sw_raw = gpio_get(pin);
        if (sw_raw == sw_state) {
            sw_ticks = 0;
        } else {
            sw_ticks++;
        }
        if (sw_ticks > (DEBOUNCE_MS / DEBOUNCE_SLEEP_MS)) {
            sw_state = sw_raw;
            sw_ticks = 0;
            if (sw_state == 0) break; // stable press confirmed, exit
        }
    }

    while (gpio_get(pin) == 0); // wait for release
}
