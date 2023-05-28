#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "video.h"

typedef struct {
    uint8_t left_click;
    uint8_t right_click;
    int16_t x;
    int16_t y;
} mouse_info_t;   

/**
 * @brief Subscribes and enables mouse interrupts.
 *
 * @return Returns bit order in interrupt mask; negative value on failure.
 */
int (mouse_subscribe_int)();

/**
 * @brief Unsubscribes mouse interrupts.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Reads a byte from a specified port related to mouse input.
 *
 * @param port The port from which to read the byte.
 * @param output Pointer to a variable where the result of the read will be stored.
 * @param mouse Boolean value that, if set, signifies that the mouse input should be read.
 * @return The result of the operation. A return value of 0 typically
 *         indicates success, while a non-zero value indicates an error.
 */
int aux_mouse_read(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Mouse interrupt handler.
 *
 * This function should be executed on each mouse interrupt. It's responsible for
 * handling the interrupt and processing its data.
 */
void (mouse_ih)();

/**
 * @brief Sync bytes related to the mouse.
 *
 * This function ensures that mouse-related bytes are correctly aligned and
 * in sync.
 */
void (mouse_sync_bytes)();

/**
 * @brief Sync mouse-related information.
 *
 * This function ensures that mouse-related information is correctly aligned,
 * in sync and processed accordingly.
 */
void (mouse_sync_info)();

/**
 * @brief Writes a command byte to a specified port related to mouse input.
 *
 * @param port The port to which the command byte will be written.
 * @param commandByte The command byte to write to the port.
 * @return The result of the operation. A return value of 0 typically
 *         indicates success, while a non-zero value indicates an error.
 */
int aux_mouse_write(uint8_t port, uint8_t commandByte);

/**
 * @brief Writes a command to the mouse.
 *
 * @param command The command to be written.
 * @return The result of the operation. A return value of 0 typically
 *         indicates success, while a non-zero value indicates an error.
 */
int (mouse_write)(uint8_t command);

#endif
