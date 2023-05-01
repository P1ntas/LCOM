#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "game.h"

typedef struct {
    int xpos, ypos, xspeed, yspeed, i;
    float angle;
} Asteroid

int create_asteroid(int xpos, int ypos, int angle);

void update_asteroid();

void destroy_asteroid();

#endif
