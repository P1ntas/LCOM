#include "bullet.h"

#define SHOT_INTERVAL 20
#define SPACE_MAKE             0x39
#define SPACE_BREAK            0xB9

extern uint8_t scancode;
extern Bullet* bullets[5];
extern Asteroid* asteroids[3];
extern mouse_info_t mouse_info;
extern vbe_mode_info_t mode_info;

extern int x_pos;
extern int y_pos;
extern int x_speed;
extern int y_speed;

int timeSinceLastShot = 0;
extern int score;

Direction get_ship_direction() {
    if (x_speed == 0 && y_speed == 0) return UP;
    else if (x_speed == 0 && y_speed > 0) return DOWN;
    else if (x_speed == 0 && y_speed < 0) return UP;
    else if (x_speed > 0 && y_speed == 0) return RIGHT;
    else if (x_speed < 0 && y_speed == 0) return LEFT;
    else if (x_speed > 0 && y_speed > 0) return DOWN_RIGHT;
    else if (x_speed > 0 && y_speed < 0) return UP_RIGHT;
    else if (x_speed < 0 && y_speed > 0) return DOWN_LEFT;
    else if (x_speed < 0 && y_speed < 0) return UP_LEFT;
    else return UP;
}

int create_bullet(int x, int y, int xspeed, int yspeed, int i) {
    Bullet* bullet = (Bullet*) malloc(sizeof(Bullet));

    if (bullet == NULL) return 1;

    switch (get_ship_direction()) {
        case UP:
            bullet->x = x + 22;
            bullet->y = y;
            bullet->xspeed = 0;
            bullet->yspeed = -10;
            break;
        case DOWN:
            bullet->x = x + 22;
            bullet->y = y + 64;
            bullet->xspeed = 0;
            bullet->yspeed = 10;
            break;
        case LEFT:
            bullet->x = x;
            bullet->y = y + 22;
            bullet->xspeed = -10;
            bullet->yspeed = 0;
            break;
        case RIGHT:
            bullet->x = x + 64;
            bullet->y = y + 22;
            bullet->xspeed = 10;
            bullet->yspeed = 0;
            break;
        case UP_LEFT:
            bullet->x = x;
            bullet->y = y;
            bullet->xspeed = -10;
            bullet->yspeed = -10;
            break;
        case UP_RIGHT:
            bullet->x = x + 64;
            bullet->y = y;
            bullet->xspeed = 10;
            bullet->yspeed = -10;
            break;
        case DOWN_LEFT:
            bullet->x = x;
            bullet->y = y + 64;
            bullet->xspeed = -10;
            bullet->yspeed = 10;
            break;
        case DOWN_RIGHT:
            bullet->x = x + 64;
            bullet->y = y + 64;
            bullet->xspeed = 10;
            bullet->yspeed = 10;
            break;
        default:
            bullet->x = x + 22;
            bullet->y = y + 22;
            bullet->xspeed = 0;
            bullet->yspeed = 0;
            break;
    }

    bullet->width = 20;
    bullet->height = 20;

    bullet->i = i;
    bullet->timer = 100;

    //draw_bullet(x, y);

    bullets[i] = bullet;

    return 0;
}

int shoot(){
    if ((scancode == SPACE_MAKE && timeSinceLastShot >= SHOT_INTERVAL) ||
    (mouse_info.left_click && timeSinceLastShot >= SHOT_INTERVAL)) {
        for (int i = 0; i < 5; i++) {
            if (bullets[i] == NULL) {
                create_bullet(x_pos, y_pos, x_speed, y_speed, i);
                timeSinceLastShot = 0;
                break;
            }
        }
    }
    return 0;
}

int update_bullets(){
    timeSinceLastShot++;
    for (int i = 0; i < 5; i++) {
        if (bullets[i] != NULL) {

            if (bullets[i]->timer == 0) {
                destroy_bullet(bullets[i]);
                continue;
            } else bullets[i]->timer--;

            bullets[i]->x += bullets[i]->xspeed;
            bullets[i]->y += bullets[i]->yspeed;

            if (bullets[i]->x > mode_info.XResolution) destroy_bullet(bullets[i]);
            else if (bullets[i]->x < 0) destroy_bullet(bullets[i]);

            else if (bullets[i]->y > mode_info.YResolution) destroy_bullet(bullets[i]);
            else if (bullets[i]->y < 0) destroy_bullet(bullets[i]);
    
            else if (bullets[i] != NULL) draw_bullet(bullets[i]->x, bullets[i]->y);
        }
    }
    return 0;
}

int destroy_bullet(Bullet* bullet) {

    bullets[bullet->i] = NULL;
    free(bullet);

    return 0;
}

int check_bullet_collision() {
    for (int i = 0; i < 5; i++) {
        if (bullets[i] != NULL) {
            for (int j = 0; j < 3; j++) {
                if (asteroids[j] != NULL) {
                    if (bullets[i]->x > asteroids[j]->x + asteroids[j]->width ||
                        bullets[i]->x + bullets[i]->width < asteroids[j]->x ||
                        bullets[i]->y > asteroids[j]->y + asteroids[j]->height ||
                        bullets[i]->y + bullets[i]->height < asteroids[j]->y) continue;
                    else {
                        score++;
                        destroy_asteroid(asteroids[j]);
                        destroy_bullet(bullets[i]);
                        return 0;
                    }
                }
            }
        }
    }
    return 0;
}
