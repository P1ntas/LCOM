#include <lcom/lcf.h>
#include "timer.h"
#include "video.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "game.h"

extern bool running;

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int setup() {
  if (timer_set_frequency(0, 60) != 0) return 1;
  if (set_frame_buffers(0x115) != 0) return 1;
  if (set_mode(0x115) != 0) return 1;
  if (timer_subscribe_inte() != 0) return 1;
  if (keyboard_subscribe_int() != 0) return 1;
  if (mouse_subscribe_int() != 0) return 1;
  if (mouse_write(0xEA) != 0) return 1;
  if (mouse_write(0xF4) != 0) return 1;
  create_bitmaps();

  return 0;
}

int shut_down() {

  if (vg_exit() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;
  if (mouse_write(0xF5) != 0) return 1;
  destroy_bitmaps();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  if (setup() != 0) return shut_down();
  refresh_screen();

  int ipc_status;
  message msg;
  while (running) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      //printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & BIT(0))    update_timer_state();
          if (msg.m_notify.interrupts & BIT(1)) update_gameState();
          if (msg.m_notify.interrupts & BIT(2))    update_mouse_state();
        }
    }
  }
  if (shut_down() != 0) return 1;

  return 0;
}
