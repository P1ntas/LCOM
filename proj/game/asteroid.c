#include "asteroid.h"

extern vbe_mode_info_t mode_info;

unsigned int radius;
extern Asteroid* asteroids[10];

int create_asteroid(int xpos, int ypos, float angle) {
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    if (asteroid == NULL) return 1;

    asteroid->xpos = xpos;
    asteroid->ypos = ypos;
    asteroid->xspeed = xspeed;
    asteroid->yspeed = yspeed;
    asteroid->angle = angle;

    draw_asteroid(x, y);

    return 0;
}

void update_asteroid() {
    for (int i = 0; i < 10; i++) {
        if (asteroids[i] != NULL) {
            asteroids[i]->xpos += asteroids[i]->xspeed;
            asteroids[i]->ypos += asteroids[i]->yspeed;

            if (asteroids[i]->xpos > mode_info.XResolution) asteroids[i]->xpos -= mode_info.XResolution;
            else if (asteroids[i]->xpos < 0) asteroids[i]->xpos += mode_info.XResolution;

            if (asteroids[i]->ypos > mode_info.YResolution) asteroids[i]->ypos -= mode_info.YResolution;
            else if (asteroids[i]->ypos < 0) asteroids[i]->ypos += mode_info.YResolution;
        }
    }
}

int destroy_asteroid(Asteroid* asteroid) {
    if (asteroid == NULL) return 1;

    asteroids[index] = NULL;
    free(asteroid);
    asteroid = NULL;

    return 0;
}
