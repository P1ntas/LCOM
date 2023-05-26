#include <lcom/lcf.h>
#include "video.h"
#include <math.h>

// Machine attributes
uint8_t *primary_vm_buffer;
uint8_t *secondary_vm_buffer;
uint8_t *drawing_vm_buffer;
uint32_t frame_buffer_size;
extern int timer_interrupts;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern MenuState menuState;
extern SystemState systemState; 

#define Double_Buffering 1 // 0 for single buffering, 1 for double buffering

// Sprites
extern Sprite *mouse;
extern Sprite *asteroid;
extern Sprite *single_player;
extern Sprite *multiplayer;
extern Sprite *controls;
extern Sprite *menu;
extern Sprite *quit;
extern Sprite *space;
extern Sprite *title;
extern Sprite *game_over;
extern Sprite *controls_menu;
extern Sprite *space_ship;
extern Sprite *bullet;
extern Sprite *num_1;
extern Sprite *num_2;
extern Sprite *num_3;
extern Sprite *num_4;
extern Sprite *num_5;
extern Sprite *num_6;
extern Sprite *num_7;
extern Sprite *num_8;
extern Sprite *num_9;
extern Sprite *num_0;
extern Sprite *score_sprite;

// Game attributes
extern int score;


int(set_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  //r86.ax = 0x4F02;
  r86.bx = mode | BIT(14);

  if (sys_int86(&r86) != 0) {
    //failed
    return 1;
  }

  return 0;
}

int (set_frame_buffer)(uint16_t mode) {

  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    printf("Failed at getting mode info.\n");
    return 1;
  }

  uint16_t vram_base = mode_info.PhysBasePtr;
  uint16_t vram_size = (mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel) / 8;

  struct minix_mem_range mr;
  mr.mr_base = vram_base;
  mr.mr_limit = vram_base + vram_size;

  int fail;
  fail = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
  if (fail) {
    return 1;
  }

  void *video_mem;
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) {
    // failed
    return 1;
  }
  printf("video mem: %p\n", video_mem);
  primary_vm_buffer = (uint8_t *) video_mem;
  printf("primary vm buffer: %p\n", primary_vm_buffer);
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
    printf("frame buffer size: %d\n", frame_buffer_size);
    if (Double_Buffering) {
        printf("Double buffering enabled\n");
        secondary_vm_buffer = (uint8_t *) malloc(frame_buffer_size);
        drawing_vm_buffer = secondary_vm_buffer;
        printf("secondary vm buffer: %p\n", secondary_vm_buffer);
        printf("drawing vm buffer: %p\n", drawing_vm_buffer);
    } else {
        printf("Double buffering disabled\n");
        drawing_vm_buffer = primary_vm_buffer;
        printf("drawing vm buffer: %p\n", drawing_vm_buffer);
    }
    return 0;
}

void swap_buffers() {
    printf("Swapping buffers\n");
    memcpy(primary_vm_buffer, secondary_vm_buffer, frame_buffer_size);
    printf("Buffers swapped\n");
}

void draw_new_frame() {
    printf("Drawing new frame\n");
    switch (menuState) {
        case MAIN_MENU:
            printf("Drawing main menu\n");
            draw_initial_menu();
            break;
        case SINGLE_PLAYER:
            printf("Drawing single player\n");
            break;
        case MULTIPLAYER:
            printf("Drawing multiplayer\n");
            break;
        case CONTROLS:
            printf("Drawing controls\n");
            draw_controls_menu();
            break;
        case END:
            printf("Drawing end\n");
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

    update_state_menu();
}

void draw_controls_menu() {
    draw_sprite_xpm(controls_menu, 0, 0);

    update_state_menu();
}

void draw_finish_menu() {
    draw_sprite_xpm(game_over, 0, 0);
}

void draw_mouse() {
    switch (menuState) {
        case MAIN_MENU: case END: case CONTROLS:
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


int draw_sprite_xpm(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == TRANSPARENT) continue;
        if (draw_pixel(x + w, y + h, current_color) != 0) {
          printf("Failed to draw pixel\n");
          return 1;
        }
      }
    }
    printf("Sprite drawn\n");
    return 0; 
}

void update_state_menu() {
    if (single_player->pressed == 1) {
        menuState = SINGLE_PLAYER;
        score = 0;
    }
    else if (multiplayer->pressed == 1) menuState = MULTIPLAYER;
    else if (controls->pressed == 1) menuState = CONTROLS;
    else if (quit->pressed == 1) systemState = EXIT;

    single_player->pressed = 0;
    multiplayer->pressed = 0;
    controls->pressed = 0;
    quit->pressed = 0;
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
            printf("0");
            draw_sprite_xpm(num_0, x, y);
            break;
        case 1:
            printf("1");
            draw_sprite_xpm(num_1, x, y);
            break;
        case 2:
            printf("2");
            draw_sprite_xpm(num_2, x, y);
            break;
        case 3:
            printf("3");
            draw_sprite_xpm(num_3, x, y);
            break;
        case 4:
            printf("4");
            draw_sprite_xpm(num_4, x, y);
            break;
        case 5:
            printf("5");
            draw_sprite_xpm(num_5, x, y);
            break;
        case 6: 
            printf("6");
            draw_sprite_xpm(num_6, x, y);
            break;
        case 7: 
            printf("7");
            draw_sprite_xpm(num_7, x, y);
            break;
        case 8:
            printf("8");
            draw_sprite_xpm(num_8, x, y);
            break;
        case 9:
            printf("9");
            draw_sprite_xpm(num_9, x, y);
            break;
        default:
            printf("Invalid number");
            break;
    }
}
