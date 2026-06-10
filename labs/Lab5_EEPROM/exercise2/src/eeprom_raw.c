#include "eeprom_raw.h"

static void mem_addr_to_bytes(uint16_t mem_addr, uint8_t *buf) {
    buf[0] = (uint8_t)(mem_addr >> 8);// High byte of address
    buf[1] = (uint8_t)(mem_addr & 0xFF); // Low byte of address
}

bool eeprom_write(uint16_t mem_addr, const uint8_t *data, size_t len) {
    uint8_t buf[2 + len];
    mem_addr_to_bytes(mem_addr, buf);
    for (size_t i = 0; i < len; i++) buf[2 + i] = data[i];

    int bytes_written = i2c_write_blocking(I2C_PORT, EEPROM_ADDR, buf, 2 + len, false);
    if (bytes_written < 0) return false;

    sleep_ms(5); // Wait 5ms for EEPROM to finish internal write

    return true;
}

bool eeprom_read(uint16_t mem_addr, uint8_t *data, size_t len) {
    uint8_t addr_buf[2];
    mem_addr_to_bytes(mem_addr, addr_buf);

    if (i2c_write_blocking(I2C_PORT, EEPROM_ADDR, addr_buf, 2, true) < 0) // Set the internal address pointer
        return false;
    if (i2c_read_blocking(I2C_PORT, EEPROM_ADDR, data, len, false) < 0) // Read data into buffer
        return false;
    return true;
}
