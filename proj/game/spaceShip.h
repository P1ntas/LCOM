#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <math.h>
#include "view/view.h"
#include "asteroid.h"
#include "direction.h"

typedef struct {
    int x, y, width, height;
    Direction direction;
    int speed, speedFactor, slowFactor;
} SpaceShip;

extern SpaceShip *spaceship;

int create_spaceship(int x, int y, SpaceShip *spaceship);

void update_spaceship();

void destroy_spaceship();

void rotate_spaceship(uint8_t scancode);

void accelerate_spaceship();

//bool ship_collides_ast(SpaceShip* spaceship, Asteroid* asteroid);

#endif
