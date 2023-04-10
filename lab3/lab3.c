#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

extern uint8_t scancode;
extern int no_calls;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g3/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g3/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(kbd_test_scan)() 
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
              printf("Scancode two bytes long!\n");
              continue;
            }
            bytes[index] = scancode;
            if (scancode & MAKE_CODE) 
            {
              printf("%02x, breakcode!\n", scancode);
            } 
            else 
            {
              printf("%02x, makecode!\n", scancode);
            }
            if (kbd_print_scancode(!(scancode & MAKE_CODE), index+1, bytes))
            {
              printf("Error: Could not print scancode\n");
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

  if (keyboard_unsubscribe_interrupts())
  {
    printf("Error: Can not unsubscribe keyboard!\n");
    return TRUE;
  }

  if(kbd_print_no_sysinb(no_calls) != 0)
  {
    printf("Error: Could not print the number of sys_inb() calls via printf!\n");
    return TRUE;
  }

  return FALSE;

}

int(kbd_test_poll)() 
{
  int index = 0;
  uint8_t bytes[2];

  while(scancode != BREAK_ESC) 
  {
    if (read_KBC_output(KBC_OUT_CMD, &scancode))
    {
      printf("Error: Could not read scancode!\n");
      tickdelay(micros_to_ticks(WAIT_KBC));
      continue;
    }

    if(scancode == TWO_BYTES)
    {
      bytes[index] = scancode;
      index++;
      printf("Scancode two bytes long!\n");
      continue;
    }
    bytes[index] = scancode;
    if (scancode & MAKE_CODE) 
    {
      printf("%02x, breakcode!\n", scancode);
    } 
    else 
    {
      printf("%02x, makecode!\n", scancode);
    }
    if (kbd_print_scancode(!(scancode & MAKE_CODE), index+1, bytes))
    {
      printf("Error: Could not print scancode\n");
      return TRUE;
    }
    index = 0;
  }

  if (keyboard_restore())
  {
    printf("Error: Could not restore interrupsts!\n");
  }

  if(kbd_print_no_sysinb(no_calls) != 0)
  {
    printf("Error: Could not print the number of sys_inb() calls via printf!\n");
    return TRUE;
  }

  return FALSE;
}

int(kbd_test_timed_scan)(uint8_t n) 
{
  int ipc_status, r, index = 0;
  int seconds_passed = 0;
  uint8_t irq_set_kbc = 0;
  uint8_t irq_set_timer = 0;
  message msg;
  uint8_t bytes[2];
  int frequency = 60; // default frequency

  if(timer_subscribe_int(&irq_set_timer))
  {
    printf("Error: Can not subscribe timer!\n");
    return TRUE;
  }

  if (keyboard_subscribe_interrupts(&irq_set_kbc))
  {
    printf("Error: Can not subscribe keyboard!\n");
    return TRUE;
  }

  while((scancode != BREAK_ESC) && (seconds_passed < n)) 
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
          if (msg.m_notify.interrupts & irq_set_timer) /* subscribed interrupt */
          { 
          /* process it */
            timer_int_handler();
            if(counter % frequency == 0) // when counter % frequency == 0 one second has passed
            {   
              timer_print_elapsed_time();
              seconds_passed++;
              printf("%d seconds passed! Time is: %d\n", seconds_passed, n);
            }
          }  
          if (msg.m_notify.interrupts & irq_set_kbc) 
          {
            kbc_ih();
            if(scancode == TWO_BYTES)
            {
              bytes[index] = scancode;
              index++;
              printf("Scancode two bytes long!\n");
              continue;
            }
            bytes[index] = scancode;
            if (scancode & MAKE_CODE) 
            {
              printf("%02x, breakcode!\n", scancode);
            } 
            else 
            {
              printf("%02x, makecode!\n", scancode);
            }
            if (kbd_print_scancode(!(scancode & MAKE_CODE), index+1, bytes))
            {
              printf("Error: Could not print scancode\n");
              return TRUE;
            }
            index = 0;
            counter = 0;
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

  if (keyboard_unsubscribe_interrupts())
  {
    printf("Error: Can not unsubscribe keyboard!\n");
    return TRUE;
  }

  if(timer_unsubscribe_int())
  {
    printf("Error: Can not unsubscribe timer!\n");
    return TRUE;
  }    
  
  if(kbd_print_no_sysinb(no_calls) != 0)
  {
    printf("Error: Could not print the number of sys_inb() calls via printf!\n");
    return TRUE;
  }

  return FALSE;
}
