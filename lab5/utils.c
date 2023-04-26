#include <lcom/lcf.h>
#include <stdint.h>

int (util_get_LSB)(uint16_t value, uint8_t *lsb) {
    if (lsb == NULL) {
        return 1;
    }

    *lsb = (value >> 8);

    return 0;
}

int (util_get_MSB)(uint16_t value, uint8_t *msb) {
    if (msb == NULL) {
        return 1;
    }

    *msb = (value);

    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
    if (value == NULL) {
        return 1;
    }

    uint32_t temp_value = 0;
    
    sys_inb(port, &temp_value);

    *value = temp_value;

    return 0;
}
