#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "video.h"

typedef struct {
    int x;
    int y;
    int xspeed;
    int yspeed;
    int width;
    int height;
    int i;
} Asteroid;

int create_asteroid(int x, int y, int i);

int update_asteroid();

int destroy_asteroid();

int check_collision(Asteroid *asteroid, int ship_x, int ship_y, int ship_width, int ship_height);

#endif
