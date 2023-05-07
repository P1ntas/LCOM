#include "view.h"

// Variáveis externas importantes à visualização do modelo e dos seus estados
uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint32_t frame_buffer_size;
extern int timer_interrupts;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern real_time_info time_info;
extern MenuState menuState;
extern SystemState systemState; 

// Objetos
extern Sprite *mouse;
extern Sprite *hand;
extern Sprite *asteroid;
extern Sprite *single_player;
extern Sprite *multiplayer;
extern Sprite *controls;
extern Sprite *menu;
extern Sprite *quit;
extern Sprite *space;
extern Sprite *title;
extern Sprite *game_over;
extern Sprite *controls_menu;
extern Sprite *space_ship;

// Alocação de memória ao(s) buffer(s)
// Se houver só um buffer, esse é o principal
// Se houver double buffering, há um secundário a alocar a mesma quantidade de memória que serve
// exclusivamente para o desenho
int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    if (DOUBLE_BUFFER) {
        secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
        drawing_frame_buffer = secondary_frame_buffer;
    } else {
        drawing_frame_buffer = main_frame_buffer;
    }
    return 0;
}

// Double buffering
// Cópia para o frame buffer principal do frame construído desde a última atualização
// Otimizaçṍes: 
// A) como o swap é uma operação muito frequente, é melhor não estar  a calcular frame_buffer_size sempre. 
// Assim opta-se por uma variável global, que é constante ao longo da execução e calculada 1 vez na linha 30.
// Poupa-se (frequência * (2 multiplicações + 1 soma + 1 divisão)) operações por cada segundo.
// B) só vale a pena dar display do RTC quando passa um segundo
void swap_buffers() {
    memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
    if (timer_interrupts % GAME_FREQUENCY == 0) display_real_time();
}

// A construção de um novo frame é baseado:
// - no estado atual do modelo (menuState, mouse_info, mode_info, buttonX->pressed...);
// - no Algoritmo do Pintor - https://pt.wikipedia.org/wiki/Algoritmo_do_pintor
// A ideia é colocar no buffer primeiro o plano mais longe do observador (a cor do fundo) e só depois 
// os objetos em cima, no caso do cursor e / ou dos botões
void draw_new_frame() {
    switch (menuState) {
        case START:
            draw_initial_menu();
            break;
        case CONTROLS:
            draw_controls_menu();
            break;
        case GAME:
            draw_game();
            break;
        case END:
            draw_finish_menu();
            break;
    }
    draw_mouse();
}

// O menu inicial é apenas um retângulo com tamanho máximo, com um smile ao centro
void draw_initial_menu() {
    draw_sprite_xpm(menu, 0, 0);
    draw_sprite_xpm(title, mode_info.XResolution/4, 20);
    draw_sprite_xpm(single_player, mode_info.XResolution/4 - 100, mode_info.YResolution/2 - 50);
    draw_sprite_xpm(controls, mode_info.XResolution/4 - 100, 3 * mode_info.YResolution/4 - 50);
    draw_sprite_xpm(multiplayer, 3 * mode_info.XResolution/4 - 100, mode_info.YResolution/2 - 50);
    draw_sprite_xpm(quit, 3 * mode_info.XResolution/4 - 100, 3 * mode_info.YResolution/4 - 50);

    update_state_menu();
}

void draw_controls_menu() {
    draw_sprite_xpm(controls_menu, 0, 0);

    update_state_menu();
}

// O menu final é apenas um retângulo com tamanho máximo, com um smile ao centro
void draw_finish_menu() {
    draw_sprite_xpm(game_over, 0, 0);
}

void draw_game() {
    draw_sprite_xpm(space, 0, 0);
    draw_sprite_xpm(space_ship, mode_info.XResolution/2, mode_info.YResolution/2);
}

// O cursor mode ter dois estados:
// - "normal", quando está no menu de início ou de fim
// - "mão", quando está no menu com os botões
void draw_mouse() {
    switch (menuState) {
        case START: case END: case CONTROLS:
            draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
            break;
        case GAME:
            draw_sprite_xpm(hand, mouse_info.x, mouse_info.y);
            break;
    }
}

int draw_asteroid(int x, int y) {

    if (x > mode_info.XResolution || x < 0 || y > mode_info.YResolution || y < 0) return 1;

    draw_sprite_xpm(asteroid, x, y);

    return 0;
}

// A função recebe um objeto Sprite proveniente de um XPM e mostra-o nas coordenadas (x, y)
// Usa as cores dinamicamente alocadas na altura da construção
// A função ignora a cor transparente do XPM para não modificar o fundo quando não é preciso
int draw_sprite_xpm(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == TRANSPARENT) continue;
        if (draw_pixel(x + w, y + h, current_color, drawing_frame_buffer) != 0) return 1;
      }
    }
    
    return 0; 
}

void update_state_menu() {

    if (single_player->pressed == 1) {
        menuState = GAME;
        
    }
    else if (multiplayer->pressed == 1) menuState = END;
    else if (controls->pressed == 1) menuState = CONTROLS;
    else if (quit->pressed == 1) systemState = EXIT;

    single_player->pressed = 0;
    multiplayer->pressed = 0;
    controls->pressed = 0;
    quit->pressed = 0;
}

// A função recebe um objeto Sprite de cor constante e mostra-o nas coordenadas (x, y)
// Usa apenas uma cor, alocada na altura da construção
// A função ignora a cor transparente do XPM para não modificar o fundo quando não é preciso
/*int draw_sprite_button(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t color = sprite->pressed ? PRESSED : sprite->color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (draw_pixel(x + w, y + h, color, drawing_frame_buffer) != 0) return 1;
      }
    }
    return 0; 
}*/

// Faz o display do tempo real num formato amigável
// No caso do Template esta função apenas retorna uma string para o ficheiro output.txt
// Em projetos pode ser mudada para invocar sprites que coloquem no ecrã os respetivos dígitos
void display_real_time() {
    printf("NOW: %d/%d/%d @%d:%d:%d\n", 2000 + time_info.year, time_info.month, time_info.day, time_info.hours, time_info.minutes, time_info.seconds);
}
