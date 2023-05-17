#include "asteroid.h"

extern vbe_mode_info_t mode_info;

unsigned int radius;
extern Asteroid* asteroids[10];

int create_asteroid(int x, int y, float angle, int index) {
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    if (asteroid == NULL) return 1;

    asteroid->x = x;
    asteroid->y = y;
    asteroid->xspeed = rand() % 10;
    asteroid->yspeed = rand() % 10;
    asteroid->angle = angle;
    asteroid->width = 64;
    asteroid->height = 64;
    asteroid->index = index;

    draw_asteroid(x, y);

    return 0;
}

int update_asteroid() {
    for (int i = 0; i < 10; i++) {
        if (asteroids[i] != NULL) {
            asteroids[i]->x += asteroids[i]->xspeed;
            asteroids[i]->y += asteroids[i]->yspeed;

            if (asteroids[i]->x > mode_info.XResolution) asteroids[i]->x -= mode_info.XResolution;
            else if (asteroids[i]->x < 0) asteroids[i]->x += mode_info.XResolution;

            if (asteroids[i]->y > mode_info.YResolution) asteroids[i]->y -= mode_info.YResolution;
            else if (asteroids[i]->y < 0) asteroids[i]->y += mode_info.YResolution;
        }
    }
    return 0;
}

int destroy_asteroid(Asteroid* asteroid) {
    if (asteroid == NULL) return 1;

    asteroids[asteroid->index] = NULL;
    free(asteroid);
    asteroid = NULL;

    return 0;
}
