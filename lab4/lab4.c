// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
#include "i8042.h"

extern struct packet mouse_packet;
extern uint8_t byte_index;

extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/g3/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g3/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) 
{
  int ipc_status, r = 0;
  uint8_t irq_set_mouse = 0;
  message msg;

  if (mouse_subscribe_interrupts(&irq_set_mouse))
  {
    printf("Error: Can not subscribe mouse!\n");
    return TRUE;
  }
  
  if (write_to_mouse(ENABLE_DEV))
  {
    printf("Error: Could not enable the device (If the device is in Stream mode (the default) and has been enabled with an Enable (0xF4) command)!\n");
    return TRUE;
  }
  
  // mouse_enable_data_reporting();

  while(cnt) 
  { /* Run until it printed cnt packets*/
    /* Get a request message */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) 
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) 
      {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_mouse) 
          { /* subscribed interrupt */
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) 
            {
              mouse_parse_packet();
              mouse_print_packet(&mouse_packet);
              byte_index = 0;
              cnt--;
            }
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
  
  if (write_to_mouse(DISABLE_DEV))
  {
    printf("Error: Could not disable the device (the host should disable the device with a Disable (0xF5) command before sending any other command)!\n");
    return TRUE;
  }

  if (mouse_unsubscribe_interrupts())
  {
    printf("Error: Can not unsubscribe mouse!\n");
    return TRUE;
  }
  
  return FALSE;
}

int (mouse_test_async)(uint8_t idle_time) 
{
  int ipc_status, r = 0;
  message msg;
  uint8_t seconds_passed = 0;
  uint8_t irq_set_mouse = 0;
  uint8_t irq_set_timer = 0;
  uint16_t timer_frequency = sys_hz();

  if(timer_subscribe_int(&irq_set_timer))
  {
    printf("Error: Can not subscribe timer!\n");
    return TRUE;
  }

  if (mouse_subscribe_interrupts(&irq_set_mouse))
  {
    printf("Error: Can not subscribe mouse!\n");
    return TRUE;
  }

  if (write_to_mouse(ENABLE_DEV))
  {
    printf("Error: Could not enable the device (If the device is in Stream mode (the default) and has been enabled with an Enable (0xF4) command)!\n");
    return TRUE;
  }

  while (seconds_passed < idle_time) 
  { 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch(_ENDPOINT_P(msg.m_source))
      {
        case HARDWARE: 
          if (msg.m_notify.interrupts & irq_set_timer) 
          { 
            timer_int_handler();
            if (counter % timer_frequency == 0) 
            {
              timer_print_elapsed_time();
              seconds_passed++;
              printf("%d seconds passed! Time is: %d\n", seconds_passed, idle_time);
            } 
          }

          if (msg.m_notify.interrupts & irq_set_mouse)
          {  
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) 
            {
              mouse_parse_packet();
              mouse_print_packet(&mouse_packet);
              byte_index = 0;
            }
            seconds_passed = 0;
            counter = 0;
          }
      }
    }
  }

  if (write_to_mouse(DISABLE_DEV))
  {
    printf("Error: Could not disable the device (the host should disable the device with a Disable (0xF5) command before sending any other command)!\n");
    return TRUE;
  }

  if(timer_unsubscribe_int())
  {
    printf("Error: Can not unsubscribe timer!\n");
    return TRUE;
  }   

  if (mouse_unsubscribe_interrupts())
  {
    printf("Error: Can not unsubscribe mouse!\n");
    return TRUE;
  }

  return FALSE;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
