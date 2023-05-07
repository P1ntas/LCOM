#include "bullet.h"

Bullet* create_bullet(int x, int y, int xspeed, int yspeed, int damage, SpaceShip owner) {
    Bullet* bullet = (Bullet*) malloc(sizeof(Bullet));

    if (bullet == NULL) return NULL;

    bullet->x = x;
    bullet->y = y;
    bullet->xspeed = xspeed;
    bullet->yspeed = yspeed;
    bullet->damage = damage;
    bullet->owner = owner;

    return bullet;
}

void update_bullet(Bullet* bullet) {
    if (bullet == NULL) return;

    bullet->x += bullet->xspeed;
    bullet->y += bullet->yspeed;

    if (bullet->x > mode_info.XResolution) bullet->x -= mode_info.XResolution;
    else if (bullet->x < 0) bullet->x += mode_info.XResolution;

    if (bullet->y > mode_info.YResolution) bullet->y -= mode_info.YResolution;
    else if (bullet->y < 0) bullet->y += mode_info.YResolution;
}

void draw_bullet(Bullet* bullet) {
    if (bullet == NULL) return;

    vg_draw_rectangle(bullet->x, bullet->y, 5, 5, 0x00FFFFFF);
}

void destroy_bullet(Bullet* bullet) {
    if (bullet == NULL) return;

    free(bullet);
    bullet = NULL;
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
}
