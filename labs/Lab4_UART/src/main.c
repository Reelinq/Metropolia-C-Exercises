#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define SW0_PIN 9

#define TIMEOUT_MS 500
#define MAX_ATTEMPTS 5
#define BUF_SIZE 64
#define DEVEUI_SIZE 32
#define DEBOUNCE_MS 30
#define DEBOUNCE_SLEEP_MS 10

typedef enum {
    STATE_WAIT_BUTTON,
    STATE_CONNECT,
    STATE_READ_VERSION,
    STATE_READ_DEVEUI,
} State;

static void debounce_button(uint pin);
static bool send_at_command(const char *cmd, const char *expected_substr, char *response_buf);
static void uart_read_line_timeout(char *buf);
static void parse_deveui(const char *response, char *out);

int main() {
    stdio_init_all();

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_hw_flow(UART_ID, false, false);

    gpio_init(SW0_PIN);
    gpio_set_dir(SW0_PIN, GPIO_IN);
    gpio_pull_up(SW0_PIN);

    State state = STATE_WAIT_BUTTON;
    int attempt = 0;
    char response[BUF_SIZE]={};
    char deveui[DEVEUI_SIZE];

    printf("Program started. Press SW_0 to begin.\n");

    while (true) {
        switch (state) {
            case STATE_WAIT_BUTTON:
                debounce_button(SW0_PIN);
                printf("Button pressed. Connecting to LoRa module...\n");
                attempt = 0;
                state = STATE_CONNECT;

            case STATE_CONNECT:
                if (attempt >= MAX_ATTEMPTS) {
                    printf("Module not responding\n");
                    attempt = 0;
                    state = STATE_WAIT_BUTTON;
                    break;
                }

                attempt++;
                printf("Sending AT (attempt %d/%d)...\n", attempt, MAX_ATTEMPTS);

                if (send_at_command("AT", "+AT: OK", response)) {
                    printf("Connected to LoRa module\n");
                    state = STATE_READ_VERSION;
                }
                break;

            case STATE_READ_VERSION:
                if (send_at_command("AT+VER", "+VER:", response)) {
                    printf("Firmware version: %s\n", response);
                    state = STATE_READ_DEVEUI;
                } else {
                    printf("Module stopped responding\n");
                    state = STATE_WAIT_BUTTON;
                }
                break;

            case STATE_READ_DEVEUI:
                if (send_at_command("AT+ID=DevEui", "DevEui", response)) {
                    parse_deveui(response, deveui);
                    printf("DevEui: %s\n", deveui);
                    state = STATE_WAIT_BUTTON;
                } else {
                    printf("Module stopped responding\n");
                    state = STATE_WAIT_BUTTON;
                }
                break;
        }
    }

    return 0;
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

static bool send_at_command(const char *cmd, const char *expected_substr, char *response_buf) {
    uart_puts(UART_ID, cmd);
    uart_puts(UART_ID, "\r\n");

    char line[BUF_SIZE];

    uart_read_line_timeout(line);
    if (strstr(line, expected_substr)) {
        strncpy(response_buf, line, BUF_SIZE);
        return true;
    }
    return false;
}

static void uart_read_line_timeout(char *buf) {
    int chars_read = 0;

    while (chars_read < BUF_SIZE - 1) {
        bool character_received = uart_is_readable_within_us(UART_ID, TIMEOUT_MS * 1000);
        if (!character_received) {
            buf[chars_read] = '\0';
            return;
        }

        char c = uart_getc(UART_ID);
        if (c == '\r') {
            buf[chars_read] = '\0';
            return;
        }

        buf[chars_read++] = c;
    }
}

static void parse_deveui(const char *response, char *out) {
    const char *deveui = strrchr(response, ' '); // Find the part after the last space
    deveui++;  // skip the space

    int output_pos = 0;
    for (int input_pos = 0; deveui[input_pos] != '\0' && output_pos < DEVEUI_SIZE - 1; input_pos++) {
        if (deveui[input_pos] == ':') continue;
        char current_char = deveui[input_pos];
        if (current_char >= 'A' && current_char <= 'F') current_char += 32;
        out[output_pos++] = current_char;
    }
    out[output_pos] = '\0';
}
