// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "video.h"

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

int (keyboard)(void) {
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

  return 0;
}


// lcom_run lab5 "init <mode> delay"
int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  if (set_mode(mode) != 0) {
    return 1;
  }

  sleep(delay);

  if (vg_exit() != 0) {
    printf("Failed at vg_exit\n");
    return 1;
  }

  return 0;
}

// lcom_run lab5 "rectangle 0x105 50 50 100 100 0x10"

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  
  if (vg_init(mode) == NULL) {
    printf("Failed at vg_init\n");
    return 1;
  }

  uint32_t right_color;
  if (get_color(color, &right_color)) return 1;

  if (vg_draw_rectangle(x, y, width, height, color) != 0){
    vg_exit();
    return 1;
  }
  
  if (keyboard() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    printf("Failed at vg_exit\n");
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  /*printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);*/

  if (vg_init(mode) == NULL) {
    return 1;
  }
  
  uint32_t width = mode_info.XResolution / no_rectangles;
  uint32_t height = mode_info.YResolution / no_rectangles;

  for (int row = 0; row < no_rectangles; row++) {
    for (int col = 0; col < no_rectangles; col++) {

      uint32_t color;
      if (mode_info.MemoryModel == 0x06) {
        uint32_t r = Red(col, step, first);
        uint32_t g = Green(col, step, first);
        uint32_t b = Blue(col, row, step, first);

        color = direct_mode(r,g,b);
      } else {
        color = indexed_mode(col, row, step, first, no_rectangles);
      }

      if (vg_draw_rectangle(row * width, col * height, width, height, color))
        return 1;
    }
  }

  if (keyboard() != 0)
    return 1;

  if (vg_exit() != 0)
    return 1;

  return 0;
}

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t *color = xpm_load(xpm, XPM_INDEXED, &img);

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (vg_draw_pixel(i + x, j + y, *color))
        return 1;
      color++;
    }
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  //printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  if (vg_init(0x105) == NULL) {
    return 1;
  }

  if (draw_xpm(xpm, x, y) != 0) {
    return 1;
  }

  if (keyboard() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  /*printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);*/

  return 1;

}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
