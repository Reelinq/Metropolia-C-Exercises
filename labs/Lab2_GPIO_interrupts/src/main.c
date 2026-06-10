#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/util/queue.h"

#define ROT_A_PIN 10
#define ROT_B_PIN 11
#define ROT_SW_PIN 12

#define PWM_CLKDIV 125
#define PWM_WRAP 999 // 125MHz / 125 / 1000 = 1kHz PWM
#define PWM_DEFAULT (PWM_WRAP / 2)
#define STEP 10
#define SLEEP_MS 10
#define DEBOUNCE_MS 30

const int LED_PINS[] = {20, 21, 22};
#define LED_COUNT (sizeof(LED_PINS) / sizeof(LED_PINS[0]))

#define CLAMP(value, low, high)  ((value) < (low) ? (low) : (value) > (high) ? (high) : (value))

queue_t rot_queue;

static void gpio_callback(uint gpio, uint32_t events) {
    int dir = gpio_get(ROT_B_PIN) ? -1 : +1;
    queue_try_add(&rot_queue, &dir);
}

static void leds_set(int32_t level) {
    for (int i = 0; i < LED_COUNT; i++) {
        uint slice   = pwm_gpio_to_slice_num(LED_PINS[i]);
        uint channel = pwm_gpio_to_channel(LED_PINS[i]);
        pwm_set_chan_level(slice, channel, level);
    }
}

int main(void) {
    queue_init(&rot_queue, sizeof(int), 10);

    // Init LEDs as PWM outputs
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&cfg, PWM_CLKDIV);
    pwm_config_set_wrap(&cfg, PWM_WRAP);

    for (int i = 0; i < LED_COUNT; i++) {
        gpio_set_function(LED_PINS[i], GPIO_FUNC_PWM);
        uint slice = pwm_gpio_to_slice_num(LED_PINS[i]);
        pwm_init(slice, &cfg, true);
    }

    // Init rotary pins
    gpio_init(ROT_A_PIN);
    gpio_set_dir(ROT_A_PIN, GPIO_IN);

    gpio_init(ROT_B_PIN);
    gpio_set_dir(ROT_B_PIN, GPIO_IN);

    gpio_init(ROT_SW_PIN);
    gpio_set_dir(ROT_SW_PIN, GPIO_IN);
    gpio_pull_up(ROT_SW_PIN);

    gpio_set_irq_enabled_with_callback(
        ROT_A_PIN,
        GPIO_IRQ_EDGE_RISE,
        true,
        &gpio_callback
    );

    // State
    int32_t bright = PWM_DEFAULT;
    bool on = false;

    // SW debounce state
    int sw_state = 1; // last stable state (1 = released)
    int sw_ticks = 0;

    leds_set(0);

    while (true) {
        int sw_raw = gpio_get(ROT_SW_PIN);

        // Debounced on/off toggle
        if (sw_raw == sw_state) {
            sw_ticks = 0;
        } else {
            sw_ticks++;
        }

        if (sw_ticks > (DEBOUNCE_MS / SLEEP_MS)) { // Check for real change
            sw_state = sw_raw;
            sw_ticks = 0;

            if (!sw_state && on && bright == 0) {
                bright = PWM_DEFAULT;
            } else if (!sw_state) {
                on = !on;
            }
        }

        int dir;
        while (queue_try_remove(&rot_queue, &dir)) {
            if (on) {
                bright = CLAMP(bright + dir * STEP, 0, PWM_WRAP);
            }
        }

        leds_set(on ? bright : 0);

        sleep_ms(SLEEP_MS);
    }
}
