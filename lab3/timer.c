#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id = TIMER0_IRQ;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  // check if timer is either 0, 1 or 2

  if (timer < 0 || timer > 2) return 1;

  // check if frequency is valid  

  if (freq > TIMER_FREQ) return 1;

  // check the status before operate

  unsigned char st = 0;
  if (timer_get_conf(timer, &st)) return 1;

  // prepare the control word
  // timer select

  unsigned char ctrl = 0;
  switch (timer)
  {
  case 0:
    ctrl |= TIMER_SEL0;
    break;

  case 1:
    ctrl |= TIMER_SEL1;
    break;

  case 2:
    ctrl |= TIMER_SEL2;
    break;
  }

  // preserve the 4 LSB of the status

  uint8_t mask = BIT(0) | BIT(1) | BIT(2) | BIT(3);
  ctrl = ctrl | TIMER_LSB_MSB | (st & mask);

  // send the control word to the timer controller

  if (sys_outb(TIMER_CTRL, ctrl)) return 1;

  // counter_init = clock/freq

  uint16_t counter_init = (uint16_t)(TIMER_FREQ / freq);

  uint8_t lsb = 0, msb = 0;

  // Split the 16 bits word in two bytes 

  if (util_get_LSB(counter_init, &lsb)) {
    printf("error in util_get_LSB\n");
    return 1;
  }

  if (util_get_MSB(counter_init, &msb)) {
    printf("error in util_get_MSB\n");
    return 1;
  }

  // port of the timer which will be configured

  int timer_port = TIMER_0 + timer;

  // Write the 8 LSB of the counter

  if (sys_outb(timer_port, lsb)) return 1;

  // Write the 8 MSB of the counter

  if (sys_outb(timer_port, msb)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  *bit_no = BIT(hook_id);
  
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;

  return 0;
    
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */
  if(sys_irqrmpolicy(&hook_id)) return 1;
  
  return 0;
}

void (timer_int_handler)() { 
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */
  counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  // check if timer is either 0, 1 or 2
  if (timer < 0 || timer > 2) return 1;
  if (st == NULL) return 1;

  // prepare to send a read back command to controle reg
  unsigned char cmd = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  // sending the command to the timer controller
  if (sys_outb(TIMER_CTRL, cmd)) return 1;

  uint8_t timer_addr = 0x40 + timer;
  printf("timer address = %x\n", timer_addr);

  // reading the state from the correspondent timer port
  if (util_sys_inb(timer_addr, st)) return 1;

  // success
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  union timer_status_field_val config;

  // todo
  // for each enum of field all/initial/mode/base/ do:

  switch (field){
    case tsf_all:
      //todo
      // copy the st to byte

      config.byte = st;
      break;
    case tsf_initial:
      //todo
      // finds the the timer value init value (LSB/MSB/LSB and MSB/invalid), bits 4, 5 of st are responsible for it

      config.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;
    case tsf_mode:
      //todo
      // find the timer counting/operating mode, bits 1, 2, 3 of st are responsible for it

      config.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      if (config.count_mode == 0x06) config.count_mode = 0x02; // exception for case 110*
      if (config.count_mode == 0x07) config.count_mode = 0x03; // exception for case 111*

      break;
    case tsf_base:
      //todo
      // find the timer counting base, bit 1 of st are responsible for it

      config.bcd = (st & TIMER_BCD);
      break;
  }

  if (timer_print_config(timer, field, config)) return 1;

  //success

  return 0;
}