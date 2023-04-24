#include <lcom/lab3.h>
#include <lcom/lcf.h>

#include "mouse.h"

extern int byte_index;
extern struct packet mouse_packet;

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

int (mouse_test_packet)(uint32_t cnt) {
  
  int ipc_status, r = 0;
  message msg;
  uint8_t irq_set = 0;

  if (mouse_subscribe_interrupts(&irq_set)) {
    printf("Error subscribing mouse interrupts");
    return 1;
  }
  if (write_to_mouse(0xF4)) {
    printf("Error enabling data reporting");
    return 1;
  }

  while (cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            printf("Test...\n");
            mouse_ih();
            if (byte_index == 3) {
              printf("Index: %d", byte_index);
              mouse_print_packet(&mouse_packet);
              byte_index = 0;
              cnt--;
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
  
  if (write_to_mouse(0xF5)) {
    printf("Error disabling data reporting");
    return 1;
  }

  if (mouse_unsubscribe_interrupts()) {
    printf("Error unsubscribing mouse interrupts");
    return 1;
  }
  printf("Test Packet succeeded... Quitting\n");
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
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
