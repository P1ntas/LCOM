#include <lcom/lcf.h>
#include <stdint.h>

int (get_lsb)(uint16_t word, uint8_t *ret) {
    if (ret == NULL) return 1;
    uint16_t temp = word;
    *ret = temp >> 4;
    return 0;
}

int (get_msb)(uint16_t word, uint8_t *ret) {
    if (ret == NULL) return 1;
    *ret = word;
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  // the value returned from sys_inb has size of 16 bits, so we cast to 8 bits
  if (value == NULL) return 1;
  
  uint32_t val = 0;
  if (sys_inb(port, &val))
    return 1;
  *value = val;

  return 0;
}
