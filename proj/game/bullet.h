#ifndef _BULLET_H_
#define _BULLET_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "view/view.h"
#include "game.h"
#include "spaceShip.h"

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
    int damage; /**< @brief damage of the bullet */
    SpaceShip owner; /**< @brief id of the owner of the bullet */
} Bullet;

/**
 * @brief Creates a bullet
 * 
 * @param x x coordinate of the bullet
 * @param y y coordinate of the bullet
 * @param xspeed speed of the bullet
 * @param yspeed speed of the bullet
 * @param damage damage of the bullet
 * @param owner id of the owner of the bullet
 * @return Bullet* pointer to the bullet created
 */
Bullet* create_bullet(int x, int y, int xspeed, int yspeed, int damage, SpaceShip owner);

/**
 * @brief Updates the bullet
 * 
 * @param bullet pointer to the bullet to be updated
 */
void update_bullet(Bullet* bullet);

/**
 * @brief Draws the bullet
 * 
 * @param bullet pointer to the bullet to be drawn
 */
void draw_bullet(Bullet* bullet);

/**
 * @brief Destroys the bullet
 * 
 * @param bullet pointer to the bullet to be destroyed
 */
void destroy_bullet(Bullet* bullet);

/**
 * @brief Checks if the bullet is out of bounds
 * 
 * @param bullet pointer to the bullet to be checked
 * @return true if the bullet is out of bounds
 * @return false if the bullet is not out of bounds
 */
bool bullet_out_of_bounds(Bullet* bullet);

/**
 * @brief Checks if the bullet collides with a spaceship
 * 
 * @param bullet pointer to the bullet to be checked
 * @param spaceship pointer to the spaceship to be checked
 * @return true if the bullet collides with the spaceship that is not the owner of the bullet
 * @return false if the bullet does not collide with the spaceship
 */
bool bullet_collides_spaceship(Bullet* bullet, SpaceShip* spaceship);

/**
 * @brief Checks if the bullet collides with a asteroid
 * 
 * @param bullet pointer to the bullet to be checked
 * @param asteroid pointer to the asteroid to be checked
 * @return true if the bullet collides with the asteroid
 * @return false if the bullet does not collide with the asteroid
 */
bool bullet_collides_asteroid(Bullet* bullet, Asteroid* asteroid);

#endif
