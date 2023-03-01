#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;

int main(int argc, char *argv[]) 
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g3/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g3/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return FALSE;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) 
{ 
  uint8_t st = 0;

  if(timer_get_conf(timer, &st))
  {
    printf("Error: Can not get/read timer configuration!\n");
    return TRUE;
  }

  if(timer_display_conf(timer, st, field))
  {
    printf("Error: Can not display timer configuration!\n", __func__);
    return TRUE;
  }
  
  printf("%s is implemented!\n", __func__);

  return FALSE;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) 
{
  if(timer_set_frequency(timer, freq))
  {
    printf("Error: Can not set the frequency of the timer!\n", __func__);
    return TRUE;
  }

  printf("%s is implemented!\n", __func__);

  return FALSE;
}

int(timer_test_int)(uint8_t time) 
{

  int ipc_status, r, seconds_passed  = 0;
  message msg;
  uint8_t irq_set = 0;
  int frequency = 60; // default frequency
  

  if(timer_subscribe_int(&irq_set))
  {
    printf("Error: Can not subscribe timer!\n");
    return TRUE;
  }
 
  while(seconds_passed < time) /* You may want to use a different condition */
  {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
    { 
        printf("Error: Driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) /* received notification */
    {
      switch (_ENDPOINT_P(msg.m_source)) 
      {
      case HARDWARE: /* hardware interrupt notification */				
        if (msg.m_notify.interrupts & irq_set) /* subscribed interrupt */
        { 
        /* process it */
          timer_int_handler();
          if(counter % frequency == 0) // when counter % frequency == 0 one second has passed
          {   
            timer_print_elapsed_time();
            seconds_passed++;
            printf("%d seconds passed! Time is: %d\n", seconds_passed, time);
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

  if(timer_unsubscribe_int())
  {
    printf("Error: Can not unsubscribe timer!\n");
    return TRUE;
  }    

  printf("%s is implemented!\n", __func__);

  return FALSE;
}
