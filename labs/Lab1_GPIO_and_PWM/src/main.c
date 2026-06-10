#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SW0_PIN 7
#define SW1_PIN 8
#define SW2_PIN 9
#define PWM_CLKDIV 125
#define PWM_WRAP 999 // 125MHz / 125 / 1000 = 1kHz PWM
#define PWM_DEFAULT (PWM_WRAP / 2)
#define STEP 10
#define SLEEP_MS 10
#define DEBOUNCE_MS 30

const int LED_PINS[] = {20, 21, 22};
#define LED_COUNT (sizeof(LED_PINS) / sizeof(LED_PINS[0]))

#define CLAMP(value, low, high)  ((value) < (low) ? (low) : (value) > (high) ? (high) : (value))

static void leds_set(int32_t level) {
    for (int i = 0; i < LED_COUNT; i++) {
        uint slice   = pwm_gpio_to_slice_num(LED_PINS[i]);
        uint channel = pwm_gpio_to_channel(LED_PINS[i]);
        pwm_set_chan_level(slice, channel, level);
    }
}

static void debounce_button(uint pin) {
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

int main(void) {
    // Init LEDs as PWM outputs
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&cfg, PWM_CLKDIV);
    pwm_config_set_wrap(&cfg, PWM_WRAP);

    for (int i = 0; i < LED_COUNT; i++) {
        gpio_set_function(LED_PINS[i], GPIO_FUNC_PWM);
        uint slice = pwm_gpio_to_slice_num(LED_PINS[i]);
        pwm_init(slice, &cfg, true);
    }

    // Init buttons
    const int BTN_PINS[] = {SW0_PIN, SW1_PIN, SW2_PIN};
    for (int i = 0; i < 3; i++) {
        gpio_init(BTN_PINS[i]);
        gpio_set_dir(BTN_PINS[i], GPIO_IN);
        gpio_pull_up(BTN_PINS[i]);
    }

    // State
    int32_t bright = PWM_DEFAULT;
    bool on = false;

    // SW1 debounce state
    int sw1_state = 1; // last stable state (1 = released)
    int sw1_ticks = 0;

    leds_set(0);

    while (true) {
        // Debounced on/off toggle for SW1
        int sw1_raw = gpio_get(SW1_PIN);
        if (sw1_raw == sw1_state) {
            sw1_ticks = 0;
        } else {
            sw1_ticks++;
        }

        if (sw1_ticks > (DEBOUNCE_MS / SLEEP_MS)) { // Check for real change
            sw1_state = sw1_raw;
            sw1_ticks = 0;

            if (!sw1_state && on && (bright == 0)) {
                bright = PWM_DEFAULT;
                leds_set(bright);
            } else if (!sw1_state) {
                on = !on;
                leds_set(on ? PWM_DEFAULT : 0);
            }
        }

        // SW0/SW2 dimming
        if (on) {
            if (!gpio_get(SW2_PIN)) bright += STEP;
            if (!gpio_get(SW0_PIN)) bright -= STEP;
            bright = CLAMP(bright, 0, PWM_WRAP);
            leds_set(bright);
        }

        sleep_ms(SLEEP_MS);
    }
}
