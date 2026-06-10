#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "config.h"
#include "ledstate.h"
#include "eeprom_raw.h"
#include "eeprom.h"
#include "leds.h"
#include "buttons.h"
#include "log.h"
#include "cli.h"


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
        gpio_set_dir(SW_PINS[i], GPIO_IN);
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

    log_write("Boot");

    while (true) {
        log_handle_command();

        for (int i = 0; i < 3; i++) {
            if (button_pressed_event(i)) {
                led_bits ^= (1u << i);
                set_led_state(&ls, led_bits);
                apply_leds(led_bits);

                eeprom_save_ledstate(&ls);

                char msg[LOG_MAX_STRING + 1];
                snprintf(msg, sizeof(msg), "LED state: LED2=%d LED1=%d LED0=%d (0x%02X)",
                        (led_bits >> 2) & 1,
                        (led_bits >> 1) & 1,
                        (led_bits) & 1,
                        led_bits);
                log_write(msg);
                print_state(led_bits, );
            }
        }
        sleep_ms(10);
    }

    return 0;
}
