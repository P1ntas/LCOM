#include "bullet.h"

extern uint8_t scancode;
extern Bullet* bullets[5];
extern uint8_t *drawing_frame_buffer;

//ship attributes
extern int x_pos;
extern int y_pos;
extern int x_speed;
extern int y_speed;
//extern Direction direction;

int create_bullet(int x, int y, int xspeed, int yspeed, int i) {
    Bullet* bullet = (Bullet*) malloc(sizeof(Bullet));

    if (bullet == NULL) return 1;

    bullet->x = x;
    bullet->y = y;
    bullet->xspeed = xspeed*2;
    bullet->yspeed = yspeed*2;
    
    // height and width of the bullet depends on the space ship direction
    bullet->width = 50;
    bullet->height = 50;


    bullet->i = i;
    bullet->timer = 100;

    draw_bullet(x, y);

    bullets[i] = bullet;

    return 0;
}

int shoot(){
    if (scancode == SPACE_MAKE) {
        for (int i = 0; i < 5; i++) {
            if (bullets[i] == NULL) {
                create_bullet(x_pos, y_pos, x_speed, y_speed, i);
                break;
            }
        }
    }
    return 0;
}

int update_bullets(){
    for (int i = 0; i < 5; i++) {
        if (bullets[i] != NULL) {

            if (bullets[i]->timer == 0) {
                destroy_bullet(bullets[i]);
                continue;
            } else bullets[i]->timer--;

            bullets[i]->x += bullets[i]->xspeed;
            bullets[i]->y += bullets[i]->yspeed;

            if (bullets[i]->x > mode_info.XResolution) bullets[i]->x -= mode_info.XResolution;
            else if (bullets[i]->x < 0) bullets[i]->x += mode_info.XResolution;

            if (bullets[i]->y > mode_info.YResolution) bullets[i]->y -= mode_info.YResolution;
            else if (bullets[i]->y < 0) bullets[i]->y += mode_info.YResolution;
        
            draw_bullet(bullets[i]->x, bullets[i]->y);
        }
    }
    return 0;
}

int destroy_bullet(Bullet* bullet) {
    //if (bullet == NULL) return 1;

    bullets[bullet->i] = NULL;
    free(bullet);

    return 0;
}

/*
void update_bullet(Bullet* bullet) {
    if (bullet == NULL) return;

    bullet->x += bullet->xspeed;
    bullet->y += bullet->yspeed;

    if (bullet->x > mode_info.XResolution) bullet->x -= mode_info.XResolution;
    else if (bullet->x < 0) bullet->x += mode_info.XResolution;

    if (bullet->y > mode_info.YResolution) bullet->y -= mode_info.YResolution;
    else if (bullet->y < 0) bullet->y += mode_info.YResolution;
}





bool bullet_out_of_bounds(Bullet* bullet) {
    if (bullet == NULL) return false;

    if (bullet->x < 0 || bullet->x > mode_info.XResolution || bullet->y < 0 || bullet->y > mode_info.YResolution) return true;
    else return false;
}

bool bullet_collides_spaceship(Bullet* bullet, SpaceShip* spaceship) {
    if (bullet == NULL || spaceship == NULL) return false;

    if (bullet->x >= spaceship->x && bullet->x <= spaceship->x + spaceship->width && bullet->y >= spaceship->y && bullet->y <= spaceship->y + spaceship->height && &bullet->owner != spaceship) return true;
    else return false;
}

bool bullet_collides_asteroid(Bullet* bullet, Asteroid* asteroid) {
    if (bullet == NULL || asteroid == NULL) return false;

    if (bullet->x >= asteroid->x && bullet->x <= asteroid->x + asteroid->width && bullet->y >= asteroid->y && bullet->y <= asteroid->y + asteroid->height) return true;
    else return false;
}*/
