#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "game.h"

typedef struct {
    int xpos, ypos, speed, speedFactor, slowFactor;
    float angle;
} Spaceship;

typedef enum {
    LEFT,
    RIGHT
} Direction;

Spaceship* create_spaceship(int xpos, int ypos, float angle);

void update_spaceship(Spaceship* spaceship);

void destroy_spaceship(Spaceship* spaceship);

void rotate_spaceship(Spaceship* spaceship, Direction direction);

void accelerate_spaceship(Spaceship* spaceship);

#endif
