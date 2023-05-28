#ifndef _BULLET_H_
#define _BULLET_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "video.h"
#include "spaceShip.h"

/**
 * 
 * @brief Enumerated type for the direction of the bullet
 * 
 */
typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
} Direction;

/**
 * @brief Gets the direction of the space ship
 * 
 * @return Direction direction of the space ship
 */
Direction get_ship_direction();

/** @defgroup bullet Bullet
 * @{
 * Functions for manipulating the bullets
 */

/**
 *  
 * @brief Struct that represents a bullet
 * 
 */
typedef struct {
    int x; /**< @brief x coordinate of the bullet */
    int y; /**< @brief y coordinate of the bullet */
    int xspeed; /**< @brief speed of the bullet */
    int yspeed; /**< @brief speed of the bullet */
    int width; /**< @brief width of the bullet */
    int height; /**< @brief height of the bullet */
    int i; /**< @brief index of the bullet */
    int timer; /**< @brief timer of the bullet */
} Bullet;

/**
 * @brief Creates a bullet
 * 
 * @param x x coordinate of the bullet
 * @param y y coordinate of the bullet
 * @param xspeed speed of the bullet
 * @param yspeed speed of the bullet
 * @param i index of the bullet
 * @return int 0 if success, 1 otherwise
 */
int create_bullet(int x, int y, int xspeed, int yspeed, int i);

/**
 * @brief Shoots a bullet, creating it and adding it to the array of bullets
 * 
 * @return int 0 if success, 1 otherwise
 */
int shoot();

/**
 * @brief Updates the bullet
 * 
 * @return int 0 if success, 1 otherwise
 */
int update_bullets();

/**
 * @brief Destroys the bullet
 * 
 * @param bullet pointer to the bullet to be destroyed
 */
int destroy_bullet(Bullet* bullet);

/**
 * @brief Checks if the bullet collides with a asteroid
 * 
 * @return int 0 if success, 1 otherwise
 */
int check_bullet_collision();

#endif
