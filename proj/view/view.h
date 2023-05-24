#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/video/graphics.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "model/sprite.h"
#include "model/model.h"

void draw_new_frame();
void draw_initial_menu();
void draw_controls_menu();
void draw_game();
void draw_finish_menu();
void draw_mouse();
int draw_asteroid(int x, int y);
void swap_buffers();
void display_real_time();
int draw_sprite_xpm(Sprite *sprite, int x, int y);
//int draw_sprite_button(Sprite *sprite, int x, int y);
int set_frame_buffers(uint16_t mode);
void update_state_menu();
void draw_score(int x, int y, int score);
void draw_number(int x, int y, int number);

#endif
