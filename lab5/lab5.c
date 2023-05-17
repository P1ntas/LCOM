// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"

vbe_mode_info_t info;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/g3/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g3/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
int (video_test_init)(uint16_t mode, uint8_t delay) 
{  
  if (vbe_get_mode_info(mode, &info))
  {
    if (vg_exit())
    {
      return TRUE;
    }
  }    

  if (vg_init(mode))
  {
    if (vg_exit())
    {
      return TRUE;
    }
  }  

  tickdelay(micros_to_ticks(delay * 1e6));

  if (vg_exit())
  {
    return TRUE;
  }

  return FALSE;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
  int ipc_status, r, index = 0;
  uint8_t irq_set = 0;
  message msg;
  uint8_t bytes[2];

  if (keyboard_subscribe_interrupts(&irq_set))
  {
    printf("Error: Can not subscribe keyboard!\n");
    return TRUE;
  }
  
  if (vg_init(mode))
  {
    if (vg_exit())
    {
      return TRUE;
    }
  }  

  if(vg_draw_rectangle(x,y,width,height,color))
  {
    return TRUE;
  }

  while(scancode != BREAK_ESC) 
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) 
    {
      printf("Error: Driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) 
    {
      switch(_ENDPOINT_P(msg.m_source))
      {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) 
          {
            kbc_ih();
            if(scancode == TWO_BYTES)
            {
              bytes[index] = scancode;
              index++;
              continue;
            }
            bytes[index] = scancode;
            if (kbd_print_scancode(!(scancode & MAKE_CODE), index+1, bytes))
            {
              return TRUE;
            }
            index = 0;
          }
          break;
      default:
        break; /* no other notifications expected: do nothing */	
      }
    } 
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }  
  }

  if (vg_exit())
  {
    return TRUE;
  }

  if (keyboard_unsubscribe_interrupts())
  {
    printf("Error: Can not unsubscribe keyboard!\n");
    return TRUE;
  }

  return FALSE;
}
