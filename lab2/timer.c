#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  uint8_t st;
  uint16_t div = TIMER_FREQ / freq;

  if (timer < 19 | timer > TIMER_FREQ) return 1;

  if (timer_get_conf(timer, &st) == 1) return 1;

  uint8_t lsb, msb;

  if (util_get_lsb(timer, &lsb) == 1) return 1;
  if (util_get_msb(timer, &msb) == 1) return 1;

  switch (timer) {
  case 0:
    st = ((st & 0x0F) | TIMER_LSB_MSB | TIMER_SEL0);
    break;

  case 1:
    st = ((st & 0x0F) | TIMER_LSB_MSB | TIMER_SEL1);
    break;

  case 2:
    st = ((st & 0x0F) | TIMER_LSB_MSB | TIMER_SEL2);
    break;
  
  default:
    return 1;
  }

  if (sys_outb(TIMER_CTRL, st) == 1 || sys_outb(TIMER_0 + timer, st) == 1) return 1;

  return 0;
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
  
  uint8_t read_back = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer));

  if (sys_outb(TIMER_CTRL, read_back) != 0) return 1;

  switch (timer) {
    case 0:
      if (util_sys_inb(TIMER_0, st) != 0) return 1;
      break;

    case 1:
      if (util_sys_inb(TIMER_1, st) != 0) return 1;
      break;

    case 2:
      if (util_sys_inb(TIMER_2, st) != 0) return 1;
      break;

    default:
      return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val val;
  
  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
  
    case tsf_initial:
      switch (st & 0x30) {
      case TIMER_LSB:
        val.in_mode = LSB_only;
        break;

      case TIMER_MSB:
        val.in_mode = MSB_only;
        break;

      case TIMER_LSB_MSB:
        val.in_mode = MSB_after_LSB;
        break;
      
      default:
      val.in_mode = INVAL_val;
        return 1;
      }
      break;

    case tsf_mode:
      switch (st & 0x0E) {
      case 0x00:
        val.count_mode = 0;
        break;

      case 0x01:
        val.count_mode = 1;
        break;

      case (0x02 | 0x06):
        val.count_mode = 2;
        break;

      case (0x03 | 0x07):
        val.count_mode = 3;
        break;
      
      case 0x04:
        val.count_mode = 4;
        break;

      case 0x05:
        val.count_mode = 5;
        break;
      
      default:
        return 1;
      }
      break;

    case tsf_base:
      switch (st & 0x01) {
        case TIMER_BCD:
          val.bcd = true;
          break;
      
        case TIMER_BIN:
          val.bcd = false;
          break;

        default:
          return 1;
      }
      break;

    default:
      return 1;
  }

  return timer_print_config(timer, field, val);
}
