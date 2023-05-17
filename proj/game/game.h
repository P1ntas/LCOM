#ifndef _GAME_H_
#define _GAME_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "stdbool.h"
#include "controller/video/graphics.h"
#include "spaceShip.h"

typedef enum {
    GAME_OVER,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_START
} GameState;

void game_init();       // Inicializa o jogo

void game_update();     // Atualiza o jogo a cada interrupção do timer

void game_draw();       // Precisaremos?

void game_destroy();    // Do we need this?


#endif
