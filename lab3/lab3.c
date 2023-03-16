#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "timer.c"

extern int counter_kbc, counter;
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
  
  int ipc_status, size;
  uint8_t irq_set;
  message msg;
  uint8_t bytes[2];

  if (keyboard_subscribe_interrupts(&irq_set) != 0) return 1;

  while(scancode != BREAK_ESC) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
            if (msg.m_notify.interrupts & irq_set) {
                kbc_ih();

                if (scancode == TWO_BYTES) {
                    bytes[size] = scancode;
                    size++;
                    continue;
                }
                bytes[size] = scancode;
                kbd_print_scancode(!(scancode & PARITY_ERROR), size + 1, bytes);
                size = 0;
            }
            break;
        default: 
            break;
        }
      }
    }

  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  if (kbd_print_no_sysinb(counter_kbc) != 0) return 1;

  return 0;
}

int(kbd_test_poll)() {

  int size;
  uint8_t bytes[2];
  
  while (scancode != BREAK_ESC) {
    printf("test\n");
    if (read_KBC_output(KBC_OUT_CMD, &scancode) == 0) {
      bytes[size] = scancode;
      kbd_print_scancode(!(scancode & PARITY_ERROR), size + 1, bytes);
      size = 0;
    }
  }
  if (kbc_restore() == 1) return 1;
  if (kbd_print_no_sysinb(counter_kbc) != 0) return 1;
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {

  uint8_t irq_set_timer, irq_set_kbc;
  message msg;
  uint8_t bytes[2];
  int ipc_status, size;

  if (timer_subscribe_int(&irq_set_timer) == 1) return 1;
  if (keyboard_subscribe_interrupts(&irq_set_kbc) == 1) return 1;

  while(scancode != BREAK_ESC && counter / 60 < n) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_kbc) {
                kbc_ih();
                /*if (scancode == TWO_BYTES) {
                    bytes[size] = scancode;
                    size++;
                    continue;
                }*/
                bytes[size] = scancode;
                kbd_print_scancode(!(scancode & PARITY_ERROR), size + 1, bytes);
                size = 0;

                counter = 0;
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler();
            }
            break;
        default: 
            break;
        }
      }
    }
  
  if (timer_unsubscribe_int() == 1) return 1;
  if (keyboard_unsubscribe_interrupts() == 1) return 1;
  if (kbd_print_no_sysinb(counter_kbc) == 1) return 1;

  return 0;
}
