#include "asteroid.h"

extern vbe_mode_info_t mode_info;

unsigned int radius;
extern Asteroid* asteroids[1];

int create_asteroid(int x, int y, int i) {
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    if (asteroid == NULL) return 1;

    asteroid->x = x;
    asteroid->y = y;
    asteroid->xspeed = (rand() % 21) - 10;
    asteroid->yspeed = (rand() % 21) - 10;
    asteroid->width = 116;
    asteroid->height = 88;
    asteroid->i = i;

    draw_asteroid(x, y);

    asteroids[i] = asteroid;

    return 0;
}

int update_asteroid() {
    for (int i = 0; i < 1; i++) {
        if (asteroids[i] != NULL) {
            asteroids[i]->x += asteroids[i]->xspeed;
            asteroids[i]->y += asteroids[i]->yspeed;

            if (asteroids[i]->x - asteroids[i]->width > mode_info.XResolution) {
                asteroids[i]->x -= mode_info.XResolution - asteroids[i]->width;
                asteroids[i]->xspeed = (rand() % 15) - 7;
                asteroids[i]->yspeed = (rand() % 15) - 7;
            }
            else if (asteroids[i]->x + asteroids[i]->width < 0) {
                asteroids[i]->x += mode_info.XResolution + asteroids[i]->width;
                asteroids[i]->xspeed = (rand() % 15) - 7;
                asteroids[i]->yspeed = (rand() % 15) - 7;
            }
            if (asteroids[i]->y - asteroids[i]->height > mode_info.YResolution) {
                asteroids[i]->y -= mode_info.YResolution - asteroids[i]->height;
                asteroids[i]->xspeed = (rand() % 15) - 7;
                asteroids[i]->yspeed = (rand() % 15) - 7;
            }
            else if (asteroids[i]->y + asteroids[i]->height < 0) {
                asteroids[i]->y += mode_info.YResolution + asteroids[i]->height;
                asteroids[i]->xspeed = (rand() % 15) - 7;
                asteroids[i]->yspeed = (rand() % 15) - 7;
            }
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

int check_collision(Asteroid *asteroid, int ship_x, int ship_y, int ship_width, int ship_height) {
    // Check if the asteroid and the ship are not overlapping (i.e., no collision)
    if (asteroid->x > ship_x + ship_width ||
        asteroid->x + asteroid->width < ship_x ||
        asteroid->y > ship_y + ship_height ||
        asteroid->y + asteroid->height < ship_y)
        return 0; // No collision

    return 1; // Collision
}
