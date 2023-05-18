#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"

typedef struct {
    int x, y, xspeed, yspeed, width, height, i;
} Asteroid;

int create_asteroid(int x, int y, int i);

int update_asteroid();

int destroy_asteroid();

#endif
