#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "KBC.h"

void (mouse_ih)();

int (mouse_subscribe_interrupts) (uint8_t *bit_no);

int (mouse_unsubscribe_interrupts)();

int (keyboard_restore)();

void (load_packet)(struct packet *packet, uint8_t bytes[]);

void (mouse_sync_bytes)();

#endif
