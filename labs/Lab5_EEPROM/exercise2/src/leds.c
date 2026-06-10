#include "leds.h"

void apply_leds(uint8_t state) {
    const int LED_PINS[] = {PIN_LED0, PIN_LED1, PIN_LED2};

    gpio_put(LED_PINS[0], (state) & 1);
    gpio_put(LED_PINS[1], (state >> 1) & 1);
    gpio_put(LED_PINS[2], (state >> 2) & 1);
}

void print_state(uint8_t state) {
    float seconds_since_startup = (float)time_us_64() / 1e6f;
    printf("[%.3f s] LED state: LED2=%d LED1=%d LED0=%d (0x%02X)\n",
           seconds_since_startup,
           (state >> 2) & 1,
           (state >> 1) & 1,
           (state) & 1,
           state);
}
