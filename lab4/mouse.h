#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

int (mouse_subscribe_interrupts)(uint8_t *bit_no);

int (keyboard_unsubscribe_interrupts)();

int (mouse_unsubscribe_interrupts)();

void (kbc_ih)();

void (mouse_ih)();

int (keyboard_restore)();

int write_to_mouse(uint8_t command);

void (mouse_parse_packet)(struct packet* pp);

#endif
