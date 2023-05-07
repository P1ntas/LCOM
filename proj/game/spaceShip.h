#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "game.h"

typedef struct {
    int x, y, speed, speedFactor, slowFactor, width, height;
    float angle;
} SpaceShip;

typedef enum {
    LEFT,
    RIGHT
} Direction;

SpaceShip* create_spaceship(int x, int y, float angle);

void update_spaceship(SpaceShip* spaceship);

void destroy_spaceship(SpaceShip* spaceship);

void rotate_spaceship(SpaceShip* spaceship, Direction direction);

void accelerate_spaceship(SpaceShip* spaceship);

bool ship_collides_ast(SpaceShip* spaceship, Asteroid* asteroid);

#endif
