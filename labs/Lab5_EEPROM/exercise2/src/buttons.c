#include "buttons.h"

bool button_pressed_event(int button_index) {
    static bool initialized = false;

    static uint8_t stable_state[NUM_BUTTONS];
    static uint8_t last_raw_state[NUM_BUTTONS];
    static absolute_time_t last_change_time[NUM_BUTTONS];

    absolute_time_t now = get_absolute_time();

    const int SW_PINS[] = {PIN_SW0, PIN_SW1, PIN_SW2};
    uint8_t raw = gpio_get(SW_PINS[button_index]);

    if (!initialized) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            stable_state[i] = 1;      // released, because buttons use pull-ups
            last_raw_state[i] = 1;
            last_change_time[i] = now;
        }
        initialized = true;
    }

    if (raw != last_raw_state[button_index]) {
        last_raw_state[button_index] = raw;
        last_change_time[button_index] = now;
    }

    if (absolute_time_diff_us(last_change_time[button_index], now) >= DEBOUNCE_MS * 1000) {
        if (raw != stable_state[button_index]) {
            stable_state[button_index] = raw;

            // Button is active-low, so 0 means pressed.
            if (stable_state[button_index] == 0) {
                return true;
            }
        }
    }

    return false;
}
