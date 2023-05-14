#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <math.h>
#include "view/view.h"
#include "asteroid.h"

typedef struct {
    int x, y, width, height;
    float angle, speed, speedFactor, slowFactor;
} SpaceShip;

typedef enum {
    LEFT,
    RIGHT
} Direction;

extern SpaceShip* spaceship;

int create_spaceship(int x, int y, float angle, SpaceShip* spaceship);

void update_spaceship(SpaceShip* spaceship);

void destroy_spaceship(SpaceShip* spaceship);

void rotate_spaceship(SpaceShip* spaceship, Direction direction);

void accelerate_spaceship(SpaceShip* spaceship);

//bool ship_collides_ast(SpaceShip* spaceship, Asteroid* asteroid);

#endif
