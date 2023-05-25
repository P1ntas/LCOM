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
extern Sprite *bullet;
extern Sprite *num_1;
extern Sprite *num_2;
extern Sprite *num_3;
extern Sprite *num_4;
extern Sprite *num_5;
extern Sprite *num_6;
extern Sprite *num_7;
extern Sprite *num_8;
extern Sprite *num_9;
extern Sprite *num_0;

extern int score;

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
        case MAIN_MENU:
            draw_initial_menu();
            break;
        case SINGLE_PLAYER:
            break;
        case MULTIPLAYER:
            break;
        case CONTROLS:
            draw_controls_menu();
            break;
        case END:
            draw_finish_menu();
            break;
        default:
            break;
    }
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


// O cursor mode ter dois estados:
// - "normal", quando está no menu de início ou de fim
// - "mão", quando está no menu com os botões
void draw_mouse() {
    switch (menuState) {
        case MAIN_MENU: case END: case CONTROLS:
            draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
            break;
        default: 
            break;
    }
}

int draw_asteroid(int x, int y) {

    if (x > mode_info.XResolution || x < 0 || y > mode_info.YResolution || y < 0) return 1;

    draw_sprite_xpm(asteroid, x, y);

    return 0;
}

int draw_bullet(int x, int y) {

    if (x > mode_info.XResolution || x < 0 || y > mode_info.YResolution || y < 0) return 1;

    draw_sprite_xpm(bullet, x, y);

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
        menuState = SINGLE_PLAYER;
        score = 0;
    }
    else if (multiplayer->pressed == 1) menuState = MULTIPLAYER;
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
// No caso do Template esta função apenas retorna uma string para o ficheiro output.draw
// Em projetos pode ser mudada para invocar sprites que coloquem no ecrã os respetivos dígitos
void display_real_time() {
    printf("NOW: %d/%d/%d @%d:%d:%d\n", 2000 + time_info.year, time_info.month, time_info.day, time_info.hours, time_info.minutes, time_info.seconds);
}

void draw_score(int x, int y, int score) {
    draw_rectangle(x, y, 150, 50, 0xFFFFFF, drawing_frame_buffer);

    int digitX = x + 110;
    int digitY = y + 10;

    int digit;
    int scoreCopy = score;

    if (score == 0) {
        draw_number(digitX, digitY, 0);
    } else {
        while (scoreCopy > 0) {
            digit = scoreCopy % 10;
            scoreCopy /= 10;
            draw_number(digitX, digitY, digit);
            digitX -= 30;
        }
    }
}

void draw_number(int x, int y, int number) {
    switch (number) {
        case 0:
            printf("0");
            draw_sprite_xpm(num_0, x, y);
            break;
        case 1:
            printf("1");
            draw_sprite_xpm(num_1, x, y);
            break;
        case 2:
            printf("2");
            draw_sprite_xpm(num_2, x, y);
            break;
        case 3:
            printf("3");
            draw_sprite_xpm(num_3, x, y);
            break;
        case 4:
            printf("4");
            draw_sprite_xpm(num_4, x, y);
            break;
        case 5:
            printf("5");
            draw_sprite_xpm(num_5, x, y);
            break;
        case 6: 
            printf("6");
            draw_sprite_xpm(num_6, x, y);
            break;
        case 7: 
            printf("7");
            draw_sprite_xpm(num_7, x, y);
            break;
        case 8:
            printf("8");
            draw_sprite_xpm(num_8, x, y);
            break;
        case 9:
            printf("9");
            draw_sprite_xpm(num_9, x, y);
            break;
        default:
            printf("Invalid number");
            break;
    }
}
