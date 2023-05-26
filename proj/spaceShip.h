#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "game.h"
#include "asteroid.h"
#include "video.h"


void update_spaceship_position();

void destroy_spaceship();

#endif
