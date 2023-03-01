#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id = TIMER0_IRQ;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) 
{
  if(freq < 19 || freq > TIMER_FREQ)
  {
    printf("Error: Frequency invalid!\n");
    return TRUE;
  }

  if(timer < 0 || timer > 2)
  {
    printf("Error: timer is not either 0, 1 or 2 -> %x!\n", timer);
    return TRUE;
  }

  uint8_t timer_configuration = 0;

  if(timer_get_conf(timer, &timer_configuration))
  {
    printf("Error: Can not get/read timer configuration!\n");
    return TRUE;
  }

  uint16_t divisor = TIMER_FREQ/freq;

  uint8_t lsb = 0;
  uint8_t msb = 0;

  if(util_get_LSB(divisor, &lsb))
  {
    printf("Error: Can not get lsb!\n");
    return TRUE;
  }

  if(util_get_MSB(divisor, &msb))
  {
    printf("Error: Can not get msb!\n");
    return TRUE;
  }

  uint8_t control_word = 0;

  switch (timer)
  {
  case 0:
    control_word |= TIMER_SEL0;
    break;
  case 1:
    control_word |= TIMER_SEL1;
    break;
  case 2:
    control_word |= TIMER_SEL2;   
    break;
  }

  control_word |= TIMER_LSB_MSB;

  control_word |= (timer_configuration & 0x0F);

  int timer_port = TIMER_0 + timer;

  if(sys_outb(TIMER_CTRL, control_word))
  {
    printf("Error: Can not load the control word into the timer control reagister!\n");
    return TRUE;
  }

  if(sys_outb(timer_port, lsb))
  {
    printf("Error: Something went wrong while writing the LSB into the counter!\n");
    return TRUE;
  }

  if(sys_outb(timer_port, msb))
  {
    printf("Error: Something went wrong while writing the MSB into the counter!\n");
    return TRUE;
  }

  printf("%s is implemented!\n", __func__);

  return FALSE;
}

int (timer_subscribe_int)(uint8_t *bit_no) 
{
  *bit_no = hook_id;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id))
  {
    printf("Error: Something went wrong while setting IRQ policy!\n");
    return TRUE;
  }

  printf("%s is implemented!\n", __func__);

  return FALSE;
}

int (timer_unsubscribe_int)() 
{
  if(sys_irqrmpolicy(&hook_id))
  {
    printf("Error: Something went wrong while unsubscribING a previous subscription of the interrupt notification associated with the specified hook_id!\n");
    return TRUE;
  }
  printf("%s is implemented!\n", __func__);

  return FALSE;
}

void (timer_int_handler)() 
{ 
  counter++;
  printf("%s is implemented, counter: %d!\n", __func__, counter);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) 
{

  if(timer < 0 || timer > 2)
  {
    printf("Error: timer is not either 0, 1 or 2 -> %x!\n", timer);
    return TRUE;
  }

  if(st == NULL)
  {
    printf("Error: st (address of memory position to be filled with the timer config) is NULL? -> %x!\n", st);
    return TRUE;
  }

  uint32_t read_back_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL, read_back_command))
  {
    printf("Error: Can not write the Read-Back command to the control register!\n");
    return TRUE;
  }

  int port = TIMER_0 + timer;

  if(util_sys_inb(port, st))
  {
    printf("Error: something happened while calling util_sys_inb!\n");
    return TRUE;
  }
  printf("%s is implemented!\n", __func__);

  return FALSE;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) 
{  
  union timer_status_field_val val;
  uint8_t initialization_mode = 0;
  uint8_t operating_mode = 0;
  
  switch (field)
  {
  case tsf_all:
    val.byte = st;
    printf("The timer status is %x: \n", val.byte);
    break;
  case tsf_initial:
    initialization_mode = (st >> 4) & 0x03;
    switch (initialization_mode)
    {
    case 0:
      val.in_mode = INVAL_val;
      printf("Latch count value command!\n");
      break;
    case 1:
      val.in_mode = LSB_only;
      printf("The timer is set to the initialization mode lobyte/lsb only!\n");
      break;
    case 2:
      val.in_mode = MSB_only;
      printf("The timer is set to the initialization mode hibyte/msb only!\n");
      break;
    case 3:
      val.in_mode = MSB_after_LSB;
      printf("The timer is set to the initialization mode lobyte/hibyte (lsb followed by msb)!\n");
      break;    
    default:
      printf("Error: Something went wrong while setting initialization mode!\n");
      return TRUE;
    }
    break;
  case tsf_mode:
    operating_mode = (st >> 1) & 0x07;
    val.count_mode = operating_mode;
    switch (operating_mode)
    {
    case 0:
      printf("The timer is set to the operating mode 0 (interrupt on terminal count)!\n");
			break;
    case 1:
      printf("The timer is set to the operating mode 1 (hardware re-triggerable one-shot)!\n");
			break;
    case 2: case 6:
      printf("The timer is set to the operating mode 2 (rate generator)!\n");
			break;
    case 3: case 7:
      printf("The timer is set to the operating mode 3 (square wave generator)!\n");
			break;
    case 4:
      printf("The timer is set to the operating mode 4 (software triggered strobe)!\n");
			break;
    case 5:
      printf("The timer is set to the operating mode 5 (hardware triggered strobe)!\n");
			break;
    default:
      printf("Error: The number received is invalid! Can not set the operating mode!\n");
      return TRUE;
    }
    break;
  case tsf_base:
    val.bcd = st & 0x01; 
    if(val.bcd == TIMER_BIN)
    {
      printf("The counting base is set to binary!\n");
    }
    else if(val.bcd == TIMER_BCD)
    {
      printf("The counting base is set to BCD (four digit)!\n");
    }
    else
    {
      printf("Error: Something went wrong while setting the counting base!\n");
    }
    break; 
  default:
    printf("Error: Something went wrong with the timer status field\n");
    return TRUE;
  }

  if(timer_print_config(timer, field, val))
  {
    printf("Error: Can not print the timer configuration!\n");
    return TRUE;
  }

  printf("%s is implemented!\n", __func__);

  return FALSE;
}
