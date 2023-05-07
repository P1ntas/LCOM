#include "game.h"

Asteroid* asteroids[10];
extern vbe_mode_info_t mode_info;

void game_init() {

}

void game_update() {
    update_spaceship();
    update_asteroid();
    update_bullet();
}

