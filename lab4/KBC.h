#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

int (read_KBC_status)(uint8_t* status);

int (read_KBC_output)(int port, uint8_t output, uint8_t mouse);

int (write_KBC_command)(int port, uint8_t commandByte);

int (write_to_mouse)(uint8_t command);

#endif /* _LCOM_KBC_H_ */
