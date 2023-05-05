#include "model.h"

// Variáveis externas importantes à construção e manipulação do modelo
extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
MenuState menuState = START;
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
Sprite *space_ship;

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
    space_ship = create_sprite_xpm((xpm_map_t) space_ship_xpm);
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
    destroy_sprite(space_ship);
}

// Na altura da interrupção há troca dos buffers e incremento do contador
void update_timer_state() {
    if (DOUBLE_BUFFER) swap_buffers();
    timer_interrupts++;
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
        case M_KEY:
            menuState = START;
            break;
        case G_KEY:
            menuState = GAME;
            break;
        case E_KEY:
            menuState = END;
        default:
            break;
    }
    draw_new_frame();
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
        draw_new_frame();
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
