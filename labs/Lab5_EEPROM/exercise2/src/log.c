#include "log.h"

static uint16_t crc16(const uint8_t *data_p, size_t length) {
    uint8_t x;
    uint16_t crc = 0xFFFF;
    while (length--) {
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    return crc;
}

static int log_find_next(void) {
    for (int i = 0; i < LOG_MAX_ENTRIES; i++) {
        uint16_t addr = LOG_START_ADDR + (i * LOG_ENTRY_SIZE);
        uint8_t first_byte;
        if (!eeprom_read(addr, &first_byte, 1))
            return -2;
        if (first_byte == 0x00)  // unused entry found
            return addr;
    }
    return -1;  // log is full
}

static bool is_valid_log_entry(const uint8_t *entry) {
    if (entry[0] == 0x00)
        return false;

    // Find null terminator, must be before index 62
    size_t str_len = 0;
    while (str_len < LOG_MAX_STRING) {
        if (entry[str_len] == '\0') break;
        str_len++;
    }
    if (entry[str_len] != '\0')  // no null terminator found
        return false;

    // Recalculate CRC
    uint16_t crc_calculated = crc16(entry, str_len + 1);

    // Read stored CRC
    uint16_t crc_stored = ((uint16_t)entry[str_len + 1] << 8) | entry[str_len + 2];

    return crc_calculated == crc_stored;
}

void log_erase_all(void) {
    uint8_t zero = 0x00;
    for (int i = 0; i < LOG_MAX_ENTRIES; i++) {
        uint16_t addr = LOG_START_ADDR + (i * LOG_ENTRY_SIZE);
        if (!eeprom_write(addr, &zero, 1)) {
            printf("EEPROM write failed at entry %d\n", i);
            return;
        }
    }
    printf("Logs erased\n");
}

bool log_write(const char *str) {
    int addr = log_find_next();
    if (addr == -2) {
        printf("EEPROM read error!\n");
        return false;
    }
    if (addr < 0) {
        printf("Log is full, erasing...\n");
        log_erase_all();
        addr = LOG_START_ADDR;
    }

    // Build 64-byte entry
    uint8_t entry[LOG_ENTRY_SIZE] = {0}; // zero fill entire entry
    size_t str_len = 0;
    while (str[str_len] != '\0' && str_len < LOG_MAX_STRING) {
        entry[str_len] = str[str_len];
        str_len++;
    }
    entry[str_len] = '\0';

    uint16_t crc = crc16(entry, str_len + 1);
    entry[str_len + 1] = (uint8_t)(crc >> 8);  // CRC high byte
    entry[str_len + 2] = (uint8_t)(crc & 0xFF); // CRC low byte

    return eeprom_write(addr, entry, LOG_ENTRY_SIZE);
}

void log_read_all(void) {
    printf("-Log entries-\n");
    bool found_any = false;

    for (int i = 0; i < LOG_MAX_ENTRIES; i++) {
        uint16_t addr = LOG_START_ADDR + (i * LOG_ENTRY_SIZE);
        uint8_t entry[LOG_ENTRY_SIZE];

        if (!eeprom_read(addr, entry, LOG_ENTRY_SIZE))  {
            printf("EEPROM read failed at entry %d\n", i);
            return;
        }

        if (!is_valid_log_entry(entry)) break;

        found_any = true;
        printf("%d: %s\n", i + 1, (char *)entry);
    }

    if (!found_any)
        printf("Log is empty\n");

    printf("-------------\n");
}
