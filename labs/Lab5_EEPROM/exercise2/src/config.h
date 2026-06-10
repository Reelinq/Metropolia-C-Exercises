#define I2C_PORT i2c0
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17
#define I2C_FREQ_HZ 100000 // 100 kHz standard mode

#define EEPROM_ADDR 0x50 // 1010(fixed) + A1=0 + A0=0 (GND) + 0(fixed) = 0b1010000
#define EEPROM_SIZE 32768u // AT24C256: 256 kilobits / 8 = 32 768 bytes

// Store ledstate at the very last two bytes of the EEPROM.
#define LEDSTATE_SIZE 2u
#define LEDSTATE_ADDR (EEPROM_SIZE - LEDSTATE_SIZE) // 32768 - 2 = 32766

#define NUM_BUTTONS 3

#define DEBOUNCE_MS 30
#define DEBOUNCE_SLEEP_MS 10

#define LOG_START_ADDR 0
#define LOG_SIZE 2048
#define LOG_ENTRY_SIZE 64
#define LOG_MAX_ENTRIES 32
#define LOG_MAX_STRING 61

#define COMMAND_BUFFER_SIZE 16

#define PIN_SW0 9
#define PIN_SW1 8
#define PIN_SW2 7

#define PIN_LED0 20
#define PIN_LED1 21
#define PIN_LED2 22
