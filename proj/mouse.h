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

int (mouse_subscribe_int)();

int (mouse_unsubscribe_int)();

int aux_mouse_read(uint8_t port, uint8_t *output, uint8_t mouse);

void (mouse_ih)();

void (mouse_sync_bytes)();

void (mouse_sync_info)();

int aux_mouse_write(uint8_t port, uint8_t commandByte);

int (mouse_write)(uint8_t command);

#endif
