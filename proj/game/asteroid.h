#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "game.h"

typedef struct {
    int x, y, xspeed, yspeed, i, width, height;
    float angle;
} Asteroid;

int create_asteroid(int x, int y, int angle);

void update_asteroid();

void destroy_asteroid();

#endif
