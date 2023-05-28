#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

/**
 * @brief Subscribes and enables Keyboard interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (keyboard_subscribe_interrupts)();

/**
 * @brief Unsubscribes Keyboard interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (keyboard_unsubscribe_interrupts)();

/**
 * @brief Keyboard interrupt handler
 * 
 */
void (kbc_ih)();

/**
 * @brief Restores the Keyboard configuration by enabling interrupts
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (keyboard_restore)();

#endif
