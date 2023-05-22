#ifndef _BULLET_H_
#define _BULLET_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "spaceShip.h"
#include "controller/video/graphics.h"

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
 * @brief Shoots a bullet
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
 * @brief Draws the bullet
 * 
 * @return int 0 if success, 1 otherwise
 */
int draw_bullet(int x, int y);

/**
 * @brief Destroys the bullet
 * 
 * @param bullet pointer to the bullet to be destroyed
 */
int destroy_bullet(Bullet* bullet);


/**
 * @brief Checks if the bullet collides with a spaceship
 * 
 * @param bullet pointer to the bullet to be checked
 * @param spaceship pointer to the spaceship to be checked
 * @return true if the bullet collides with the spaceship that is not the owner of the bullet
 * @return false if the bullet does not collide with the spaceship
 */
//bool bullet_collides_spaceship(Bullet* bullet, SpaceShip* spaceship);

/**
 * @brief Checks if the bullet collides with a asteroid
 * 
 * @param bullet pointer to the bullet to be checked
 * @param asteroid pointer to the asteroid to be checked
 * @return true if the bullet collides with the asteroid
 * @return false if the bullet does not collide with the asteroid
 */
//bool bullet_collides_asteroid(Bullet* bullet, Asteroid* asteroid);

#endif
