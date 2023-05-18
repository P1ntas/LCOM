#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/video/graphics.h"
#include "controller/rtc/rtc.h"
#include "game/spaceShip.h"
#include "xpm/hand.xpm"
#include "xpm/mouse.xpm"
#include "xpm/asteroid.xpm"
#include "xpm/single_player.xpm"
#include "xpm/multiplayer.xpm"
#include "xpm/controls.xpm"
#include "xpm/menu.xpm"
#include "xpm/quit.xpm"
#include "xpm/space.xpm"
#include "xpm/title.xpm"
#include "xpm/game-over.xpm"
#include "xpm/controls_menu.xpm"
#include "xpm/space_ship_1.xpm"
#include "xpm/space_ship_2.xpm"
#include "xpm/space_ship_3.xpm"
#include "xpm/space_ship_4.xpm"
#include "xpm/space_ship_5.xpm"
#include "xpm/space_ship_6.xpm"
#include "xpm/space_ship_7.xpm"
#include "xpm/space_ship_8.xpm"
#include "view/view.h"
#include "model/sprite.h"
#include "config.h"

typedef enum {
    RUNNING,
    EXIT,
} SystemState;

typedef enum {
    MAIN_MENU,
    SINGLE_PLAYER,
    MULTIPLAYER,
    CONTROLS,
    END
} MenuState;

void update_timer_state();
void update_keyboard_state();
void update_mouse_state();
void update_buttons_state();
void update_rtc_state();
void setup_sprites();
void destroy_sprites();

#endif

