#include "model.h"

// Variáveis externas importantes à construção e manipulação do modelo
extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
MenuState menuState = MAIN_MENU;
extern MouseInfo mouse_info;
extern vbe_mode_info_t mode_info;
extern real_time_info time_info;

// Objetos a construir e manipular com a mudança de estados
Sprite *mouse;
Sprite *hand;
Sprite *asteroid;
Sprite *single_player;
Sprite *multiplayer;
Sprite *controls;
Sprite *menu;
Sprite *quit;
Sprite *space;
Sprite *title;
Sprite *game_over;
Sprite *controls_menu;
Sprite *space_ship1;
Sprite *space_ship2;
Sprite *space_ship3;
Sprite *space_ship4;
Sprite *space_ship5;
Sprite *space_ship6;
Sprite *space_ship7;
Sprite *space_ship8;
Sprite *bullet;
Sprite *num_1;
Sprite *num_2;
Sprite *num_3;
Sprite *num_4;
Sprite *num_5;
Sprite *num_6;
Sprite *num_7;
Sprite *num_8;
Sprite *num_9;
Sprite *num_0;

Asteroid* asteroids[1];
Bullet* bullets[10];
int score = 0;

// Contador de interrupções do timer
int timer_interrupts = 0;

// Criação dos objetos via XPM e via comum
void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    hand = create_sprite_xpm((xpm_map_t) hand_xpm);
    asteroid = create_sprite_xpm((xpm_map_t) asteroid_xpm);
    single_player = create_sprite_xpm((xpm_map_t) single_player_xpm);
    multiplayer = create_sprite_xpm((xpm_map_t) multiplayer_xpm);
    controls = create_sprite_xpm((xpm_map_t) controls_xpm);
    menu = create_sprite_xpm((xpm_map_t)menu_xpm);
    quit = create_sprite_xpm((xpm_map_t) quit_xpm);
    space = create_sprite_xpm((xpm_map_t) space_xpm);
    title = create_sprite_xpm((xpm_map_t) title_xpm);
    game_over = create_sprite_xpm((xpm_map_t) game_over_xpm);
    controls_menu = create_sprite_xpm((xpm_map_t) controls_menu_xpm);
    space_ship1 = create_sprite_xpm((xpm_map_t) space_ship_xpm_1);
    space_ship2 = create_sprite_xpm((xpm_map_t) space_ship_xpm_2);
    space_ship3 = create_sprite_xpm((xpm_map_t) space_ship_xpm_3);
    space_ship4 = create_sprite_xpm((xpm_map_t) space_ship_xpm_4);
    space_ship5 = create_sprite_xpm((xpm_map_t) space_ship_xpm_5);
    space_ship6 = create_sprite_xpm((xpm_map_t) space_ship_xpm_6);
    space_ship7 = create_sprite_xpm((xpm_map_t) space_ship_xpm_7);
    space_ship8 = create_sprite_xpm((xpm_map_t) space_ship_xpm_8);
    bullet = create_sprite_xpm((xpm_map_t) bullet_xpm);
    num_1 = create_sprite_xpm((xpm_map_t) num_1_xpm);
    num_2 = create_sprite_xpm((xpm_map_t) num_2_xpm);
    num_3 = create_sprite_xpm((xpm_map_t) num_3_xpm);
    num_4 = create_sprite_xpm((xpm_map_t) num_4_xpm);
    num_5 = create_sprite_xpm((xpm_map_t) num_5_xpm);
    num_6 = create_sprite_xpm((xpm_map_t) num_6_xpm);
    num_7 = create_sprite_xpm((xpm_map_t) num_7_xpm);
    num_8 = create_sprite_xpm((xpm_map_t) num_8_xpm);
    num_9 = create_sprite_xpm((xpm_map_t) num_9_xpm);
    num_0 = create_sprite_xpm((xpm_map_t) num_0_xpm);
}

// É boa prática antes de acabar o programa libertar a memória alocada
void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(hand);
    destroy_sprite(asteroid);
    destroy_sprite(single_player);
    destroy_sprite(multiplayer);
    destroy_sprite(controls);
    destroy_sprite(menu);
    destroy_sprite(quit);
    destroy_sprite(space);
    destroy_sprite(title);
    destroy_sprite(game_over);
    destroy_sprite(controls_menu);
    destroy_sprite(space_ship1);
    destroy_sprite(space_ship2);
    destroy_sprite(space_ship3);
    destroy_sprite(space_ship4);
    destroy_sprite(space_ship5);
    destroy_sprite(space_ship6);
    destroy_sprite(space_ship7);
    destroy_sprite(space_ship8);
    destroy_sprite(bullet);
    destroy_sprite(num_1);
    destroy_sprite(num_2);
    destroy_sprite(num_3);
    destroy_sprite(num_4);
    destroy_sprite(num_5);
    destroy_sprite(num_6);
    destroy_sprite(num_7);
    destroy_sprite(num_8);
    destroy_sprite(num_9);
    destroy_sprite(num_0);
}

// Na altura da interrupção há troca dos buffers e incremento do contador
void update_timer_state() {
    timer_interrupts++;
    //constanstly update the ship position
    draw_new_frame();
    if (menuState == SINGLE_PLAYER || menuState == MULTIPLAYER) {
        update_spaceship_position();
        update_asteroid();
        shoot();
        update_bullets();
        check_bullet_collision();
        draw_score(mode_info.XResolution - 150, 0, score);
    }
    draw_mouse();
    if (DOUBLE_BUFFER) swap_buffers();
}

// Como o Real Time Clock é um módulo mais pesado, 
// devemos só atualizar os valores quando passa um segundo
void update_rtc_state() {
    if (timer_interrupts % GAME_FREQUENCY == 0) rtc_update_time();
}

// Sempre que uma nova tecla é pressionada há avaliação do scancode.
// No caso do Template o teclado influencia:
// - o systemState: se Q for pressionado, leva ao fim do programa
// - o menuState: se S, G, E forem pressionados, leva a um dos menus (start, game, end) disponíveis
void update_keyboard_state() {
    (kbc_ih)();
    switch (scancode) {
        case Q_KEY:
            systemState = EXIT;
            break;
        case BREAK_ESC:
            menuState = MAIN_MENU;
            break;
        case G_KEY:
            menuState = SINGLE_PLAYER;
            score = 0;
            break;
        case M_KEY:
            menuState = MAIN_MENU;
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

// Sempre que há um novo pacote completo do rato
// - muda o seu estado interno (x, y, left_pressed, right_pressed) - mouse_sync_info();
// - pode mudar o estado do botão por baixo dele - update_buttons_state();
void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    if (byte_index == 3) {
        mouse_sync_info();
        update_buttons_state();
        byte_index = 0;
    }
}

// Se o rato tiver o botão esquerdo pressionado (mouse_info.left_click) então
// muda o estado do botão no mesmo quadrante
// Senão, todos os botões voltam a não estar pressionados (buttonX->pressed = 0;)
void update_buttons_state() {

    if (mouse_info.left_click) {
        if (mouse_info.x >= mode_info.XResolution/4 - 100 && mouse_info.y >= mode_info.YResolution/2 - 50
                && (mouse_info.x <= mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= mode_info.YResolution/2 - 50 + 48))
            single_player->pressed = 1;

        if ((mouse_info.x >= 3 * mode_info.XResolution/4 - 100) && (mouse_info.y >= mode_info.YResolution/2 - 50)
                && (mouse_info.x <= 3 * mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= mode_info.YResolution/2 - 50 + 48))
            multiplayer->pressed = 1;

        if ((mouse_info.x >= mode_info.XResolution/4 - 100) && (mouse_info.y >= 3 * mode_info.YResolution/4 - 50)
                && (mouse_info.x <= mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= 3 * mode_info.YResolution/4 - 50 + 48))
            controls->pressed = 1;

        if ((mouse_info.x >= 3 * mode_info.XResolution/4 - 100) && (mouse_info.y >= 3 * mode_info.YResolution/4 - 50)
                && (mouse_info.x <= 3 * mode_info.XResolution/4 - 100 + 192) && (mouse_info.y <= 3 * mode_info.YResolution/4 - 50 + 48))
            quit->pressed = 1;

    } else {
        single_player->pressed = 0;
        multiplayer->pressed = 0;
        controls->pressed = 0;
        quit->pressed = 0;
    }
}

