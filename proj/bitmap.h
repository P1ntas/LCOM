#ifndef _LCOM_BITMAP_H_
#define _LCOM_BITMAP_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * 
 * @brief Struct that represents a bitmap
 * 
 */
typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
} BitMap; 

/**
 * @brief Creates a bitmap
 * 
 * @param bitmap xpm_map_t of the bitmap
 * @return BitMap* pointer to the bitmap
 */
BitMap *create_bitmap(xpm_map_t bitmap);

/**
 * @brief Destroys a bitmap
 * 
 * @param bitmap pointer to the bitmap
 */
void destroy_bitmap(BitMap *bitmap);

#endif
