#include "spaceShip.h"

#define SPEED 5

// Variáveis externas importantes à construção e manipulação do modelo
extern uint8_t scancode;
extern vbe_mode_info_t mode_info;
extern Sprite *space_ship1;
extern Sprite *space_ship2;
extern Sprite *space_ship3;
extern Sprite *space_ship4;
extern Sprite *space_ship5;
extern Sprite *space_ship6;
extern Sprite *space_ship7;
extern Sprite *space_ship8;
extern Sprite *space;
extern Sprite *hand;
int x_pos = 0;
int y_pos = 0;
int x_speed = 0;
int y_speed = 0;

extern MenuState menuState;
extern Asteroid* asteroids[1];

// spaceship already initialized in main.c at the center of the screen 

void update_spaceship_position() 
{
    if (W_MAKE == scancode) y_speed = -SPEED;
    if (W_BREAK == scancode) 
    {
        if (y_speed == -SPEED) y_speed = 0;
    }
    if (S_MAKE == scancode) y_speed = SPEED;
    if (S_BREAK == scancode)
    {
        if (y_speed == SPEED) y_speed = 0;
    }
    if (A_MAKE == scancode) x_speed = -SPEED;
    if (A_BREAK == scancode)
    {
        if (x_speed == -SPEED) x_speed = 0;
    }
    if (D_MAKE == scancode) x_speed = SPEED;
    if (D_BREAK == scancode)
    {
        if (x_speed == SPEED) x_speed = 0;
    }

    x_pos += x_speed;
    y_pos += y_speed;

    if (x_pos > mode_info.XResolution) x_pos -= mode_info.XResolution;
    else if (x_pos < 0) x_pos += mode_info.XResolution;
    if (y_pos > mode_info.YResolution) y_pos -= mode_info.YResolution;
    else if (y_pos < 0) y_pos += mode_info.YResolution;

    draw_sprite_xpm(space, 0, 0);
    if (x_speed > 0 && y_speed > 0) draw_sprite_xpm(space_ship8, x_pos, y_pos); // diagonal movement to the right and down 
    else if (x_speed < 0 && y_speed < 0) draw_sprite_xpm(space_ship7, x_pos, y_pos); // diagonal movement to the left and up 
    else if (x_speed > 0 && y_speed < 0) draw_sprite_xpm(space_ship6, x_pos, y_pos); // diagonal movement to the right and up 
    else if (x_speed < 0 && y_speed > 0) draw_sprite_xpm(space_ship5, x_pos, y_pos); // diagonal movement to the left and down 
    else if (x_speed > 0 && y_speed == 0) draw_sprite_xpm(space_ship4, x_pos, y_pos); // horizontal movement to the right 
    else if (x_speed < 0 && y_speed == 0) draw_sprite_xpm(space_ship3, x_pos, y_pos); // horizontal movement to the left
    else if (x_speed == 0 && y_speed > 0) draw_sprite_xpm(space_ship2, x_pos, y_pos); // vertical movement down 
    else if (x_speed == 0 && y_speed < 0) draw_sprite_xpm(space_ship1, x_pos, y_pos); // vertical movement up 
    else draw_sprite_xpm(space_ship1, x_pos, y_pos); // default sprite

    for (int i = 0; i < 1; i++) {
        if (asteroids[i] != NULL) {
            if (check_collision(asteroids[i], x_pos, y_pos, space_ship1->width, space_ship1->height)) {
                menuState = END;
            }
        }
    }
}
