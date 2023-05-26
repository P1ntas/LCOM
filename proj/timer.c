#include <lcom/lcf.h>
#include <stdint.h>
#include "timer.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  uint8_t st = 0;
  uint16_t div = TIMER_FREQ / freq;

  if (freq < 19 || freq > TIMER_FREQ) return 1;

  uint8_t lsb, msb;

  if (util_get_LSB(div, &lsb) == 1) return 1;
  if (util_get_MSB(div, &msb) == 1) return 1;

  if (timer_get_conf(timer, &st) == 1) return 1;

  switch (timer) {
    case 0:
      st = ((st & 0x0F) | TIMER_SEL0 | TIMER_LSB_MSB);
      break;

    case 1:
      st = ((st & 0x0F) | TIMER_SEL1 | TIMER_LSB_MSB);
      break;

    case 2:
      st = ((st & 0x0F) | TIMER_SEL2 | TIMER_LSB_MSB);
      break;
  
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, st) == 1) return 1;
  if (sys_outb(TIMER_0 + timer, lsb) == 1) return 1;
  if (sys_outb(TIMER_0 + timer, msb) == 1) return 1;

  return 0;
}

int (timer_subscribe_interrupts)() {
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&hook_id);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  uint8_t read_back = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

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
