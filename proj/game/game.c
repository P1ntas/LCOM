#include "game.h"

extern SpaceShip *spaceship;
extern GameState game_state;

void game_init() {
    game_state = GAME_RUNNING;
    create_spaceship(mode_info.XResolution / 2, mode_info.YResolution / 2, spaceship);
}

void game_update() {
    update_spaceship(spaceship);
}

void game_destroy() {
    destroy_spaceship(spaceship);
    game_state = GAME_START;
}

