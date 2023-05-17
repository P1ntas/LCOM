#include "spaceShip.h"


// Variáveis externas importantes à construção e manipulação do modelo
extern uint8_t scancode;
extern vbe_mode_info_t mode_info;
extern Sprite *space_ship;
extern Sprite *space;
extern Sprite *hand;
int x_pos = 0;
int y_pos = 0;
int x_speed = 0;
int y_speed = 0;


// spaceship already initialized in main.c at the center of the screen 

void update_spaceship_position() 
{
    switch (scancode)
    {
    case W_MAKE:
        y_speed = -10;
        break;
    case W_BREAK:
        if (y_speed == -10) y_speed = 0;
        break;
    case S_MAKE:
        y_speed = 10;
        break;
    case S_BREAK:
        if (y_speed == 10) y_speed = 0;
        break;
    case A_MAKE:
        x_speed = -10;
        break;
    case A_BREAK:
        if (x_speed == -10) x_speed = 0;
        break;
    case D_MAKE:
        x_speed = 10;
        break;
    case D_BREAK:
        if (x_speed == 10) x_speed = 0;
        break;
    default:
        break;
    }
    x_pos += x_speed;
    y_pos += y_speed;
    draw_sprite_xpm(space, 0, 0);
    draw_sprite_xpm(space_ship, x_pos + mode_info.XResolution/2, y_pos + mode_info.YResolution/2);
}

void destroy_spaceship() {
    if (space_ship == NULL) return;

    free(space_ship);
    space_ship = NULL;
}
