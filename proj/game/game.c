#include "game.h"

void game_init() {
    game_state = GAME_START;
    create_spaceship(mode_info.XResolution / 2, mode_info.YResolution / 2, 0, &spaceship);
}

void game_update() {
    update_spaceship();
}

void game_destroy() {
    destroy_spaceship(&spaceship);
}

