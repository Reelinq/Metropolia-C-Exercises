#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "config.h"
#include "ledstate.h"
#include "eeprom_raw.h"
#include "eeprom.h"
#include "leds.h"
#include "buttons.h"

int main(void) {
    stdio_init_all();

    i2c_init(I2C_PORT, I2C_FREQ_HZ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    const int LED_PINS[] = {PIN_LED0, PIN_LED1, PIN_LED2};

    for (int i = 0; i < 3; i++) {
        gpio_init(LED_PINS[i]);
        gpio_set_dir(LED_PINS[i], GPIO_OUT);
        gpio_put(LED_PINS[i], 0);
    }

    const int SW_PINS[] = {PIN_SW0, PIN_SW1, PIN_SW2};

    for (int i = 0; i < 3; i++) {
        gpio_init(SW_PINS[i]);
        gpio_pull_up(SW_PINS[i]);
    }

    ledstate ls;
    uint8_t led_bits;

    if (eeprom_load_ledstate(&ls)) {
        led_bits = ls.state;
        printf("Valid EEPROM state found: 0x%02X\n", led_bits);
    } else {
        led_bits = 0x02; // Only middle led on
        set_led_state(&ls, led_bits);
        printf("No valid EEPROM state – using default: 0x%02X\n", led_bits);
    }

    apply_leds(led_bits);
    print_state(led_bits);

    while (true) {
        for (int i = 0; i < 3; i++) {
            if (!gpio_get(SW_PINS[i])) {        /* button pressed? */
                debounce_button(SW_PINS[i]);    /* wait for clean press + release */

                led_bits ^= (1u << i);
                set_led_state(&ls, led_bits);
                apply_leds(led_bits);

                print_state(led_bits);
                eeprom_save_ledstate(&ls);
            }
        }
        sleep_ms(10);
    }

    return 0;
}
