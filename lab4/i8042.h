#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC. Needs to be completed.
 */

#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       2
#define IRQ_LINE_MOUSE  12

#define BREAK_ESC       0x81
#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)
#define FOWARD_REQ      0xD4
#define ACK             0XFA

#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER  BIT(1)

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define WAIT_KBC        20000
#define MAX_ATTEMPTS    10

#define ENABLE_DEV      0xF4
#define DISABLE_DEV     0xF5

#define PRESSED_LB      BIT(0)
#define PRESSED_MB      BIT(1)
#define PRESSED_RB      BIT(2)
#define CONTROL         BIT(3)
#define DELTA_X         BIT(4)
#define DELTA_Y         BIT(5)
#define OVERFLOW_X      BIT(6)
#define OVERFLOW_Y      BIT(7)

/* MACROS PARA BOLEANOS */

#define TRUE    1
#define FALSE   0

/**@}*/

#endif /* _LCOM_I8042_H_ */
