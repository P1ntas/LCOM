#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "video.h"

/**
 * @brief Struct that represents an asteroid
 * 
 */
typedef struct {
    int x;
    int y;
    int xspeed;
    int yspeed;
    int width;
    int height;
    int i;
} Asteroid;

/**
 * @brief Creates an asteroid
 * 
 * @param x X coordinate of the asteroid
 * @param y Y coordinate of the asteroid
 * @param i Index of the asteroid
 * @return int 0 if success, 1 otherwise
 */
int create_asteroid(int x, int y, int i);

/**
 * @brief Updates the asteroids
 * 
 * @return int 0 if success, 1 otherwise
 */
int update_asteroid();

/**
 * @brief Destroys the asteroid
 * 
 * @param asteroid pointer to the asteroid to be destroyed
 * @return int 0 if success, 1 otherwise
 */
int destroy_asteroid(Asteroid* asteroid);

/**
 * @brief Checks if the asteroid collides with the spaceship
 * 
 * @param ship_x X coordinate of the spaceship
 * @param ship_y Y coordinate of the spaceship
 * @param ship_width Width of the spaceship
 * @param ship_height Height of the spaceship
 * @return int 0 if success, 1 otherwise
 */
int check_collision(Asteroid *asteroid, int ship_x, int ship_y, int ship_width, int ship_height);

#endif
