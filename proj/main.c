#include <lcom/lcf.h>
#include "timer.h"
#include "video.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "game.h"

extern SystemState systemState;

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int setup() {
  if (timer_set_frequency(TIMER, 60) != 0) return 1;
  if (set_frame_buffers(0x115) != 0) return 1;
  if (set_mode(0x115) != 0) return 1;
  setup_sprites();
  if (timer_subscribe_interrupts() != 0) return 1;
  if (keyboard_subscribe_interrupts() != 0) return 1;
  if (mouse_subscribe_interrupts() != 0) return 1;
  if (mouse_write(ENABLE_STREAM_MODE) != 0) return 1;
  if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;

  return 0;
}

int teardown() {

  if (vg_exit() != 0) return 1;
  destroy_sprites();
  if (timer_unsubscribe_interrupts() != 0) return 1;
  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  if (mouse_unsubscribe_interrupts() != 0) return 1;
  if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  if (setup() != 0) return teardown();
  draw_new_frame();

  int ipc_status;
  message msg;
  while (systemState == RUNNING) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & BIT(0))    update_timer_state();
          if (msg.m_notify.interrupts & BIT(1)) update_keyboard_state();
          if (msg.m_notify.interrupts & BIT(2))    update_mouse_state();
        }
    }
  }
  if (teardown() != 0) return 1;

  return 0;
}
