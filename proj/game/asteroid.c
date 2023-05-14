#include "asteroid.h"

extern vbe_mode_info_t mode_info;

unsigned int radius;
extern Asteroid* asteroids[10];

int create_asteroid(int x, int y, float angle) {
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    if (asteroid == NULL) return 1;

    asteroid->x = x;
    asteroid->y = y;
    asteroid->xspeed = 1;
    asteroid->yspeed = 1;
    asteroid->angle = angle;
    asteroid->width = 64;
    asteroid->height = 64;

    draw_asteroid(x, y);

    return 0;
}

void update_asteroid() {
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
}

int destroy_asteroid(Asteroid* asteroid) {
    if (asteroid == NULL) return 1;
    int index = -1;
    for (int i = 0; i < 10; i++) {
        if (asteroids[i] == asteroid) {
            index = i;
            break;
        }
    }
    asteroids[index] = NULL;
    free(asteroid);
    asteroid = NULL;

    return 0;
}
