#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <stdio.h>

typedef enum {
    UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
} Direction;

void getVector(Direction direction, int *vector);

#endif
