#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  // check if timer is either 0, 1 or 2
  if (timer < 0 || timer > 2) return 1;
  if (st == NULL) return 1;

  // prepare to send a read back command to controle reg
  unsigned char cmd = (TIMER_RB_CMD | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer));

  // sending the command to the timer controller
  sys_outb(TIMER_CTRL, cmd);

  uint8_t timer_addr = 0x40 + timer;
  printf("timer address = %x\n", timer_addr);
  util_sys_inb(timer_addr, st);


  // success
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */
  union timer_status_field_val conf;

  // todo




  if (timer_print_config(timer, field, conf)) return 1;

  //success
  return 0;
}
