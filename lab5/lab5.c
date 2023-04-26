// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <lcom/video_gr.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"

// video_gr
vbe_mode_info_t mode_info;

// keyboard
extern int counter;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  /* To be completed */
  // printf("%s(0x%03x, %u): under construction\n", __func__, mode, delay);

  // vbe_get_mode_info(mode, &mode_info);

  if (vg_init(mode) == NULL) {
    printf("Failed at vg_init\n");
    return 1;
  }

  tickdelay(micros_to_ticks(delay * 1e6));

  if (vg_exit() != 0) {
    printf("Failed at vg_exit\n");
    return 1;
  }

  return 0;
}

// lcom_run lab5 "rectangle 0x105 50 50 100 100 0x10"

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  /* To be completed */
  /*printf("%s(0x%03X, %u, %u, %u, %u, 0x%08x): under construction\n",
         __func__, mode, x, y, width, height, color);*/

  if (vg_init(mode) == NULL) {
    printf("Failed at vg_init\n");
    return 1;
  }

  uint16_t w, h;
  w = (x + width > mode_info.XResolution) ? mode_info.XResolution - x : width;
  h = (y + height > mode_info.YResolution) ? mode_info.YResolution - y : height;

  if (x < mode_info.XResolution && y < mode_info.YResolution) {
    if (vg_draw_rectangle(x, y, w, h, color) != 0){
      return 1;
    }
  } else {
    printf("X or Y out of bound.\n");
    return 1;
  }
    

  // keyboard code
  int ipc_status;
  uint8_t irq_set;
  message msg;
  uint8_t code[2];
  int index = 0;

  if (keyboard_subscribe_interrupts(&irq_set) != 0)
    return 1;

  while (scancode != 0x81 /*ESC*/) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (scancode == 0xE0 /*TWO_BYTES*/) {
              code[index] = scancode;
              index += 1;
              continue;
            }
            code[index] = scancode;
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

  if (vg_exit() != 0) {
    printf("Failed at vg_exit\n");
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
