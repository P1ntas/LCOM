#include "asteroid.h"

extern vbe_mode_info_t mode_info;

unsigned int radius;
extern Asteroid* asteroids[10];

int create_asteroid(int x, int y, int i) {
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    if (asteroid == NULL) return 1;

    asteroid->x = x;
    asteroid->y = y;
    asteroid->xspeed = (rand() % 21) - 10;
    asteroid->yspeed = (rand() % 21) - 10;
    asteroid->width = 64;
    asteroid->height = 64;
    asteroid->i = i;

    draw_asteroid(x, y);

    asteroids[i] = asteroid;

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
        else create_asteroid((rand() % 2) ? mode_info.XResolution : 0, rand() % mode_info.YResolution, i);

        draw_asteroid(asteroids[i]->x, asteroids[i]->y);
    }
    return 0;
}

int destroy_asteroid(Asteroid* asteroid) {
    if (asteroid == NULL) return 1;

    asteroids[asteroid->i] = NULL;
    free(asteroid);
    asteroid = NULL;

    return 0;
}
