#include <lcom/lab3.h>
#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"

extern int counter;
extern int no_sys_call;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  uint8_t irq_set;
  message msg;
  uint8_t code[2];
  int index = 0;

  if (keyboard_subscribe_interrupts(&irq_set) != 0)
    return 1;

  while (scancode != BREAK_ESC) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (scancode == TWO_BYTES) {
              code[index] = scancode;
              printf("Two bytes scancode!\n");
              printf("0x%02x ", scancode);
              index += 1;
              continue;
            }
            code[index] = scancode;
            if (kbd_print_scancode(!(scancode & MAKE_CODE), index + 1, code)) {
              printf("Error printing scancode\n");
              return 1;
              index = 0;
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0)
    return 1;
  if (kbd_print_no_sysinb(no_sys_call) != 0)
    return 1;

  return 0;
}

int(kbd_test_poll)() {

  int index = 0;
  uint8_t code[2];

  while (scancode != BREAK_ESC) {
    if (read_KBC_output(KBC_OUT_CMD, &scancode)) {
      printf("Error: Could not read scancode!\n");
      tickdelay(micros_to_ticks(WAIT_KBC));
      continue;
    }
    code[index] = scancode;
    kbd_print_scancode(!(scancode & MAKE_CODE), index + 1, code);
    index = 0;
  }
  if (keyboard_restore()) return 1;


  if (kbd_print_no_sysinb(no_sys_call)) return 1;
  

  return 0;
}

int(kbd_test_timed_scan)(uint8_t time) {

    int index = 0;
    uint8_t code[2];

    uint8_t irq_set_kbc = 0, irq_set_timer = 0;
    if (keyboard_subscribe_interrupts(&irq_set_kbc))
        return 1;
    if (timer_subscribe_int(&irq_set_timer))
        return 1;

    int ipc_status, r, seconds = 0, frequency = 60;
    message msg;

    while ((scancode != BREAK_ESC) && (seconds < time)) { /* You may want to use a different condition */
              /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                             /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
                        timer_int_handler();
                        if (counter % frequency == 0) {
                            timer_print_elapsed_time();
                            seconds++;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_kbc) { /* subscribed interrupt */
                        kbc_ih();
                        if (scancode == TWO_BYTES) {
                            code[index] = scancode;
                            index += 1;
                            continue;
                        }
                        code[index] = scancode;
                        if (scancode & MAKE_CODE) {
                            printf("Make code: 0x%02x\n", scancode);
                        }
                        else {
                            printf("Break code: 0x%02x\n", scancode);
                        }
                        if (kbd_print_scancode(!(scancode & MAKE_CODE), index + 1, code)) {
                            printf("Error printing scancode\n");
                            return 1;
                        }
                        index = 0;
                        counter = 0;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        }
        else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }

    if (keyboard_unsubscribe_interrupts() != 0)
        return 1;
    if (timer_unsubscribe_int() != 0)
        return 1;
    if (kbd_print_no_sysinb(no_sys_call) != 0)
        return 1;
    
  //...
  return 1;
}
