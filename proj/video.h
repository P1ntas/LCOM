#ifndef __VIDEO_H
#define __VIDEO_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/mouse/mouse.h"
#include "bitmap.h"
#include "game.h"


vbe_mode_info_t mode_info;

/**
 * @brief Sets the graphic mode to the one specified in the argument
 * 
 * @param mode Submode of the graphic mode
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (set_mode)(uint16_t mode);

/**
 * @brief Sets the frame buffer to the one specified in the argument
 * 
 * @param mode Mode of the frame buffer
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Draws a pixel in the specified coordinates with the specified color
 * 
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixel
 * @param frame_buffer Frame buffer to draw the pixel
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a line in the specified coordinates with the specified length and color
 * 
 * @param x X coordinate of the line
 * @param y Y coordinate of the line
 * @param len Length of the line
 * @param color Color of the line
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle in the specified coordinates with the specified width, height and color
 * 
 * @param x X coordinate of the rectangle
 * @param y Y coordinate of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Prepares the frame buffers, using a single buffer or a double buffer
 * 
 * @param mode Mode of the frame buffer
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int set_frame_buffers(uint16_t mode);

/**
 * @brief Draws a new frame, depending on the current state of the menu
 */
void refresh_screen();

/**
 * @brief Draws the initial menu
 */
void draw_initial_menu();

/**
 * @brief Draws the finish menu
 */
void draw_game_over();

/**
 * @brief Draws the mouse
 */
void draw_mouse();

/**
 * @brief Draws an asteroid in the specified coordinates
 * 
 * @param x X coordinate of the asteroid
 * @param y Y coordinate of the asteroid
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_asteroid(int x, int y);

/**
 * @brief Draws a bullet in the specified coordinates
 * 
 * @param x X coordinate of the bullet
 * @param y Y coordinate of the bullet
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_bullet(int x, int y);

/**
 * @brief Draws a sprite in the specified coordinates
 * 
 * @param bitmap BitMap to be drawn
 * @param x X coordinate of the sprite
 * @param y Y coordinate of the sprite
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_bitmap(BitMap *bitmap, int x, int y);

/**
 * @brief Draws the score field in the specified coordinates
 * 
 * @param x X coordinate of the score
 * @param y Y coordinate of the score
 * @param score Score to be drawn
 */
void draw_score(int x, int y, int score);

/**
 * @brief Draws the number in the specified coordinates
 * 
 * @param x X coordinate of the number
 * @param y Y coordinate of the number
 * @param number Number to be drawn
 */
void draw_number(int x, int y, int number);



#endif
