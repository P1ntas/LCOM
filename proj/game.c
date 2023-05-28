#include "game.h"

bool running = TRUE;
extern uint8_t scancode;
extern mouse_info_t mouse_info;
extern uint8_t byte_index;
extern vbe_mode_info_t mode_info;
gameState menuState = MENU;
extern uint8_t *primary_vm_buffer;
extern uint8_t *secondary_vm_buffer;
extern uint32_t frame_buffer_size;

BitMap *mouse;
BitMap *asteroid;
BitMap *single_player;
BitMap *multiplayer;
BitMap *controls;
BitMap *menu;
BitMap *quit;
BitMap *space;
BitMap *title;
BitMap *game_over;
BitMap *controls_menu;
BitMap *space_ship1;
BitMap *space_ship2;
BitMap *space_ship3;
BitMap *space_ship4;
BitMap *space_ship5;
BitMap *space_ship6;
BitMap *space_ship7;
BitMap *space_ship8;
BitMap *bullet;
BitMap *num_1;
BitMap *num_2;
BitMap *num_3;
BitMap *num_4;
BitMap *num_5;
BitMap *num_6;
BitMap *num_7;
BitMap *num_8;
BitMap *num_9;
BitMap *num_0;
BitMap *score_sprite;
BitMap *msg;

// Game attributes
Asteroid* asteroids[3];
Bullet* bullets[5];
int score = 0;

// Spaceship attributes
int x_pos = 400;
int y_pos = 300;
int x_speed = 0;
int y_speed = 0;

int counter_timer = 0;

void create_bitmaps() {
    mouse = create_bitmap((xpm_map_t) mouse_xpm);
    asteroid = create_bitmap((xpm_map_t) asteroid_xpm);
    single_player = create_bitmap((xpm_map_t) single_player_xpm);
    multiplayer = create_bitmap((xpm_map_t) multiplayer_xpm);
    controls = create_bitmap((xpm_map_t) controls_xpm);
    menu = create_bitmap((xpm_map_t)menu_xpm);
    quit = create_bitmap((xpm_map_t) quit_xpm);
    space = create_bitmap((xpm_map_t) space_xpm);
    title = create_bitmap((xpm_map_t) title_xpm);
    game_over = create_bitmap((xpm_map_t) game_over_xpm);
    controls_menu = create_bitmap((xpm_map_t) controls_menu_xpm);
    space_ship1 = create_bitmap((xpm_map_t) space_ship_xpm_1);
    space_ship2 = create_bitmap((xpm_map_t) space_ship_xpm_2);
    space_ship3 = create_bitmap((xpm_map_t) space_ship_xpm_3);
    space_ship4 = create_bitmap((xpm_map_t) space_ship_xpm_4);
    space_ship5 = create_bitmap((xpm_map_t) space_ship_xpm_5);
    space_ship6 = create_bitmap((xpm_map_t) space_ship_xpm_6);
    space_ship7 = create_bitmap((xpm_map_t) space_ship_xpm_7);
    space_ship8 = create_bitmap((xpm_map_t) space_ship_xpm_8);
    bullet = create_bitmap((xpm_map_t) bullet_xpm);
    num_1 = create_bitmap((xpm_map_t) num_1_xpm);
    num_2 = create_bitmap((xpm_map_t) num_2_xpm);
    num_3 = create_bitmap((xpm_map_t) num_3_xpm);
    num_4 = create_bitmap((xpm_map_t) num_4_xpm);
    num_5 = create_bitmap((xpm_map_t) num_5_xpm);
    num_6 = create_bitmap((xpm_map_t) num_6_xpm);
    num_7 = create_bitmap((xpm_map_t) num_7_xpm);
    num_8 = create_bitmap((xpm_map_t) num_8_xpm);
    num_9 = create_bitmap((xpm_map_t) num_9_xpm);
    num_0 = create_bitmap((xpm_map_t) num_0_xpm);
    score_sprite = create_bitmap((xpm_map_t) score_xpm);
    msg = create_bitmap((xpm_map_t) msg_xpm);
}

void destroy_bitmaps() {
    destroy_bitmap(mouse);
    destroy_bitmap(asteroid);
    destroy_bitmap(single_player);
    destroy_bitmap(multiplayer);
    destroy_bitmap(controls);
    destroy_bitmap(menu);
    destroy_bitmap(quit);
    destroy_bitmap(space);
    destroy_bitmap(title);
    destroy_bitmap(game_over);
    destroy_bitmap(controls_menu);
    destroy_bitmap(space_ship1);
    destroy_bitmap(space_ship2);
    destroy_bitmap(space_ship3);
    destroy_bitmap(space_ship4);
    destroy_bitmap(space_ship5);
    destroy_bitmap(space_ship6);
    destroy_bitmap(space_ship7);
    destroy_bitmap(space_ship8);
    destroy_bitmap(bullet);
    destroy_bitmap(num_1);
    destroy_bitmap(num_2);
    destroy_bitmap(num_3);
    destroy_bitmap(num_4);
    destroy_bitmap(num_5);
    destroy_bitmap(num_6);
    destroy_bitmap(num_7);
    destroy_bitmap(num_8);
    destroy_bitmap(num_9);
    destroy_bitmap(num_0);
    destroy_bitmap(score_sprite);
    destroy_bitmap(msg);
}

void update_timer_state() {
    counter_timer++;

    refresh_screen();
    if (menuState == SINGLE_PLAYER || menuState == MULTIPLAYER) {
        update_spaceship_position();
        update_asteroid();
        shoot();
        update_bullets();
        check_bullet_collision();
        draw_score(mode_info.XResolution - 300, 0, score);
    }
    draw_mouse();
    memcpy(primary_vm_buffer, secondary_vm_buffer, frame_buffer_size);
}

void update_gameState() {
    (kbc_ih)();
    switch (scancode) {
        case Q_KEY:
            running = FALSE;
            break;
        case BREAK_ESC:
            menuState = MENU;
            break;
        case G_KEY:
            menuState = SINGLE_PLAYER;
            game_reset();
            break;
        case M_KEY:
            menuState = MENU;
            break;
        case C_KEY:
            menuState = CONTROLS;
            break;
        case E_KEY:
            menuState = END;
            break;
        default:
            break;  
    }
}

void update_mouse_state() {
    (mouse_ih)();

    mouse_sync_bytes();

    if (byte_index == 3) {
        mouse_sync_info();
        update_menu();
        byte_index = 0;
    }
}

void update_menu() {

    if (mouse_info.left_click) {
        if (mouse_info.x >= mode_info.XResolution/4 - 100 && mouse_info.y >= mode_info.YResolution/2 - 50
                && (mouse_info.x <= mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= mode_info.YResolution/2 - 50 + 48)) {
            menuState = SINGLE_PLAYER;
            game_reset();
        }

        if ((mouse_info.x >= 3 * mode_info.XResolution/4 - 100) && (mouse_info.y >= mode_info.YResolution/2 - 50)
                && (mouse_info.x <= 3 * mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= mode_info.YResolution/2 - 50 + 48))
            menuState = MULTIPLAYER;

        if ((mouse_info.x >= mode_info.XResolution/4 - 100) && (mouse_info.y >= 3 * mode_info.YResolution/4 - 50)
                && (mouse_info.x <= mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= 3 * mode_info.YResolution/4 - 50 + 48))
            menuState = CONTROLS;

        if ((mouse_info.x >= 3 * mode_info.XResolution/4 - 100) && (mouse_info.y >= 3 * mode_info.YResolution/4 - 50)
                && (mouse_info.x <= 3 * mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= 3 * mode_info.YResolution/4 - 50 + 48))
            running = FALSE;

    }
}

void game_reset(){
    score = 0;
    for (int i = 0; i < 5; i++) {
        if (bullets[i] != NULL)
            destroy_bullet(bullets[i]);
    }
    for (int i = 0; i < 3; i++) {
        if (asteroids[i] != NULL)
            destroy_asteroid(asteroids[i]);
    }
    x_pos = 400;
    y_pos = 300;
}
