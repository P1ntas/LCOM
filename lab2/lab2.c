#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  /* To be implemented by the students */
  /* printf("%s is not yet implemented!\n", __func__); */

  // to test type lcom_run lab2 "config <0,1,2> all|init|mode|base"

  unsigned char st = 0;
  if (timer_get_conf(timer, &st)) return 1;
  if (timer_display_conf(timer, st, field)) return 1;

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  // lcom_run lab2 "time <0,1,2> <frequency>"

  if (timer_set_frequency(timer, freq)) return 1;

  return 0;
}

int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  // lcom_run lab2 "int <0,1,2> <time - seconds>"
  //in this function should test the timer counting the number of seconds

  // setting up the timer frequency

  const int frequency = 60;
  // timer_set_frequency(0, frequency);

  uint8_t irq_set = 0;

  //subscribe the timer

  if (timer_subscribe_int(&irq_set)) return 1;

  // declaring the variables

  int ipc_status, r, seconds = 0;
  message msg;

  while( seconds < time ) { /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r); 
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            /* process it */
            //handler
            timer_int_handler();
            if ((counter % frequency) == 0) { /* second elapsed */
              timer_print_elapsed_time();
              seconds++;
              printf("%d... \n", seconds);
            }
          }
        break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  // unsubscribe the timer at the end
  if (timer_unsubscribe_int()) return 1;

  return 0;
}
