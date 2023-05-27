#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * 
 * @brief Struct that represents a sprite
 * 
 */
typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
    uint32_t color;
    uint8_t pressed;
} Sprite; 

/**
 * @brief Creates a sprite
 * 
 * @param sprite xpm_map_t of the sprite
 * @return Sprite* pointer to the sprite
 */
Sprite *create_sprite_xpm(xpm_map_t sprite);

/**
 * @brief Destroys a sprite
 * 
 * @param sprite pointer to the sprite
 */
void destroy_sprite(Sprite *sprite);

#endif
