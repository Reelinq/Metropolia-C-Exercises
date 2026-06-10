#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "motor.h"

#define IN1 2
#define IN2 3
#define IN3 6
#define IN4 13

void motor_set(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);

void motor_off() {
    motor_set(0, 0, 0, 0);
}

void motor_step() {
    const uint8_t HALF_STEP[8][4] = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1},
    };

    static uint8_t step_index = 0;
    step_index = (step_index + 1) % 8;
    motor_set(
        HALF_STEP[step_index][0],
        HALF_STEP[step_index][1],
        HALF_STEP[step_index][2],
        HALF_STEP[step_index][3]
    );
    sleep_ms(1); // Motor needs time to physically move
}

void motor_set(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4) {
    gpio_put(IN1, in1);
    gpio_put(IN2, in2);
    gpio_put(IN3, in3);
    gpio_put(IN4, in4);
}

void init_motor() {
    uint8_t motor_pins[] = {IN1, IN2, IN3, IN4};

    for (uint8_t i = 0; i < 4; i++) {
        gpio_init(motor_pins[i]);
        gpio_set_dir(motor_pins[i], GPIO_OUT);
        gpio_put(motor_pins[i], 0);
    }
}
