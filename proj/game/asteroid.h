#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "game.h"

typedef struct {
    int x, y, xspeed, yspeed, i, width, height, index;
    float angle;
} Asteroid;

int create_asteroid(int x, int y, int angle, int index);

int update_asteroid();

int destroy_asteroid();

#endif
