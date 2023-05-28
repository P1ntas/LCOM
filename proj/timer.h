#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

/**
 * @brief Sets the frequency of the timer
 * 
 * @param timer Timer whose frequency is to be changed
 * @param freq Frequency to be set
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes the timer interrupts, using sys_irqsetpolicy
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (timer_subscribe_inte)();

/**
 * @brief Unsubscribes the timer interrupts, using sys_irqrmpolicy
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (timer_unsubscribe_int)();

/**
 * @brief Gets the configuration of the timer
 * 
 * @param timer Timer whose configuration is to be read
 * @param st Address of memory position to be filled with the timer configuration
 * 
 * @return int 0 if successful, 1 otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif
