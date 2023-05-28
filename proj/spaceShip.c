#include "spaceShip.h"

#define SPEED 5

extern uint8_t scancode;
extern vbe_mode_info_t mode_info;
extern BitMap *space_ship1;
extern BitMap *space_ship2;
extern BitMap *space_ship3;
extern BitMap *space_ship4;
extern BitMap *space_ship5;
extern BitMap *space_ship6;
extern BitMap *space_ship7;
extern BitMap *space_ship8;
extern BitMap *space;
extern int x_pos;
extern int y_pos;
extern int x_speed;
extern int y_speed;

extern gameState menuState;
extern Asteroid* asteroids[3];

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

    draw_bitmap(space, 0, 0);
    if (x_speed > 0 && y_speed > 0) draw_bitmap(space_ship8, x_pos, y_pos); // diagonal movement to the right and down 
    else if (x_speed < 0 && y_speed < 0) draw_bitmap(space_ship7, x_pos, y_pos); // diagonal movement to the left and up 
    else if (x_speed > 0 && y_speed < 0) draw_bitmap(space_ship6, x_pos, y_pos); // diagonal movement to the right and up 
    else if (x_speed < 0 && y_speed > 0) draw_bitmap(space_ship5, x_pos, y_pos); // diagonal movement to the left and down 
    else if (x_speed > 0 && y_speed == 0) draw_bitmap(space_ship4, x_pos, y_pos); // horizontal movement to the right 
    else if (x_speed < 0 && y_speed == 0) draw_bitmap(space_ship3, x_pos, y_pos); // horizontal movement to the left
    else if (x_speed == 0 && y_speed > 0) draw_bitmap(space_ship2, x_pos, y_pos); // vertical movement down 
    else if (x_speed == 0 && y_speed < 0) draw_bitmap(space_ship1, x_pos, y_pos); // vertical movement up 
    else draw_bitmap(space_ship1, x_pos, y_pos); // default sprite

    for (int i = 0; i < 3; i++) {
        if (asteroids[i] != NULL) {
            if (check_collision(asteroids[i], x_pos, y_pos, space_ship1->width, space_ship1->height)) {
                menuState = END;
            }
        }
    }
}
