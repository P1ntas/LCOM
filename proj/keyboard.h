#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Subscribes and enables Keyboard interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (keyboard_subscribe_int)();

/**
 * @brief Unsubscribes Keyboard interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Reads a byte from a specified port.
 *
 * This function attempts to read a byte from a port of the
 * auxiliary keyboard. If the read is successful, the result
 * is placed in the location pointed to by the output parameter.
 *
 * @param port The port from which to read the byte.
 * @param output Pointer to a variable where the result of the read will be stored.
 * @return The result of the operation. A return value of 0 typically
 *         indicates success, while a non-zero value indicates an error.
 */
int aux_keyboard_read(uint8_t port, uint8_t *output);

/**
 * @brief Keyboard interrupt handler
 * 
 */
void (kbc_ih)();

/**
 * @brief Writes a command byte to a specified port.
 *
 * This function attempts to write a command byte to a port of the
 * auxiliary keyboard.
 *
 * @param port The port to which the command byte will be written.
 * @param commandByte The command byte to write to the port.
 * @return The result of the operation. A return value of 0 typically
 *         indicates success, while a non-zero value indicates an error.
 */
int aux_keyboard_write(uint8_t port, uint8_t commandByte);

/**
 * @brief Restores the Keyboard configuration by enabling interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (keyboard_restore)();

#endif
