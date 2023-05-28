#include <lcom/lcf.h>
#include "video.h"
#include <math.h>

uint8_t *primary_vm_buffer;
uint8_t *secondary_vm_buffer;
uint8_t *drawing_vm_buffer;
uint32_t frame_buffer_size;
extern int counter_timer;
extern mouse_info_t mouse_info;
extern gameState menuState;
extern bool running; 

#define Double_Buffering 1 // 0 for single buffering, 1 for double buffering

extern BitMap *mouse;
extern BitMap *asteroid;
extern BitMap *single_player;
extern BitMap *multiplayer;
extern BitMap *controls;
extern BitMap *menu;
extern BitMap *quit;
extern BitMap *space;
extern BitMap *title;
extern BitMap *game_over;
extern BitMap *controls_menu;
extern BitMap *space_ship;
extern BitMap *bullet;
extern BitMap *num_1;
extern BitMap *num_2;
extern BitMap *num_3;
extern BitMap *num_4;
extern BitMap *num_5;
extern BitMap *num_6;
extern BitMap *num_7;
extern BitMap *num_8;
extern BitMap *num_9;
extern BitMap *num_0;
extern BitMap *score_sprite;
extern BitMap *msg;

extern int score;


int(set_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = mode | BIT(14);

  if (sys_int86(&r86) != 0) return 1;

  return 0;
}

int (set_frame_buffer)(uint16_t mode) {

  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    return 1;
  }

  unsigned int vram_size = (mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel) / 8;

  struct minix_mem_range mr;
  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  int fail;
  fail = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
  if (fail) {
    return 1;
  }

  
  primary_vm_buffer = (uint8_t *) vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (primary_vm_buffer == NULL) {
    return 1;
  }
  return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= mode_info.XResolution || y >= mode_info.YResolution)
    return 1;

  unsigned bpp = (mode_info.BitsPerPixel + 7) / 8;

  unsigned int index = (mode_info.XResolution * y + x) * bpp;

  memcpy(&drawing_vm_buffer[index], &color, bpp);

  return 0;

}

int (draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (draw_pixel(x + i, y, color) != 0)
      return 1;
  }
  return 0;

}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = y; i < height + y; i++) {
    if (draw_line(x, i, width, color) != 0)
      return 1;
  }
  return 0;
}

int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    if (Double_Buffering) {
        secondary_vm_buffer = (uint8_t *) malloc(frame_buffer_size);
        drawing_vm_buffer = secondary_vm_buffer;
    } else {
        drawing_vm_buffer = primary_vm_buffer;
    }
    return 0;
}

void swap_buffers() {
    memcpy(primary_vm_buffer, secondary_vm_buffer, frame_buffer_size);
}

void refresh_screen() {
    //printf("Drawing new frame\n");
    switch (menuState) {
        case MENU:
            //printf("Drawing main menu\n");
            draw_initial_menu();
            break;
        case SINGLE_PLAYER:
            //printf("Drawing single player\n");
            break;
        case MULTIPLAYER:
            //printf("Drawing multiplayer\n");
            break;
        case CONTROLS:
            //printf("Drawing controls\n");
            draw_sprite_xpm(controls_menu, 0, 0);
            break;
        case END:
            //printf("Drawing end\n");
            draw_finish_menu();
            break;
        default:
            break;
    }
}


void draw_initial_menu() {
    draw_sprite_xpm(menu, 0, 0);
    draw_sprite_xpm(title, mode_info.XResolution/4, 20);
    draw_sprite_xpm(single_player, mode_info.XResolution/4 - 100, mode_info.YResolution/2 - 50);
    draw_sprite_xpm(controls, mode_info.XResolution/4 - 100, 3 * mode_info.YResolution/4 - 50);
    draw_sprite_xpm(multiplayer, 3 * mode_info.XResolution/4 - 100, mode_info.YResolution/2 - 50);
    draw_sprite_xpm(quit, 3 * mode_info.XResolution/4 - 100, 3 * mode_info.YResolution/4 - 50);
}

void draw_finish_menu() {
    draw_sprite_xpm(game_over, 0, 0);
    draw_score(mode_info.XResolution/3 - 15, 3 * mode_info.YResolution/4, score);
    draw_sprite_xpm(msg, mode_info.XResolution/4, 3 * mode_info.YResolution/4 + 60);
}

void draw_mouse() {
    switch (menuState) {
        case MENU: case END: case CONTROLS:
            draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
            break;
        default: 
            break;
    }
}

int draw_asteroid(int x, int y) {

    if (x > mode_info.XResolution || x < 0 || y > mode_info.YResolution || y < 0) return 1;

    draw_sprite_xpm(asteroid, x, y);

    return 0;
}

int draw_bullet(int x, int y) {

    if (x > mode_info.XResolution || x < 0 || y > mode_info.YResolution || y < 0) return 1;

    draw_sprite_xpm(bullet, x, y);

    return 0;
}


int draw_sprite_xpm(BitMap *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == 0xFFFFFE) continue;
        if (draw_pixel(x + w, y + h, current_color) != 0) {
          return 1;
        }
      }
    }
    return 0; 
}

void draw_score(int x, int y, int score) {
    draw_rectangle(x, y, 300, 50, 0xFFFFFF);
    draw_sprite_xpm(score_sprite, x+10, y+10);

    int digitX = x + 270;
    int digitY = y + 10;

    int digit;
    int scoreCopy = score;

    if (score == 0) {
        draw_number(digitX, digitY, 0);
    } else {
        while (scoreCopy > 0) {
            digit = scoreCopy % 10;
            scoreCopy /= 10;
            draw_number(digitX, digitY, digit);
            digitX -= 30;
        }
    }
}

void draw_number(int x, int y, int number) {
    switch (number) {
        case 0:
            draw_sprite_xpm(num_0, x, y);
            break;
        case 1:
            draw_sprite_xpm(num_1, x, y);
            break;
        case 2:
            draw_sprite_xpm(num_2, x, y);
            break;
        case 3:
            draw_sprite_xpm(num_3, x, y);
            break;
        case 4:
            draw_sprite_xpm(num_4, x, y);
            break;
        case 5:
            draw_sprite_xpm(num_5, x, y);
            break;
        case 6: 
            draw_sprite_xpm(num_6, x, y);
            break;
        case 7: 
            draw_sprite_xpm(num_7, x, y);
            break;
        case 8:
            draw_sprite_xpm(num_8, x, y);
            break;
        case 9:
            draw_sprite_xpm(num_9, x, y);
            break;
        default:
            break;
    }
}
