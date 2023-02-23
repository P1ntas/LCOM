#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  // LSB -> least-significant bits
  // this function should update (*lsb) with the 8 least-significant bits of (val)
  *lsb = val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  // MSB -> most significant bits
  // this function should update (*msb) with the 8 most significant bits of (val)
  // to do this, we have to right shift 8 bits to (val)

  uint16_t temp = val;
  *msb = (temp >> 8);
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */
  uint32_t val = 0;
  sys_inb(port, &val);
  *value = val;

  return 0;
}
