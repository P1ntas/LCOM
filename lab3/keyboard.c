#include<lcom/lcf.h>

#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"


/* ------ keyboard.c ------ */
int keyboard_hook_id = 1;
uint8_t scancode = 0;

// subscribe interrupts
int keyboard_subscribe_interrupts (uint8_t *bit_no)
{
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(keyboard_hook_id);
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, 
                          &keyboard_hook_id);
}

// unsubscribe interrupts
int keyboard_unsubscribe_interrupts ()
{
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
  if (read_KBC_output(KBC_OUT_CMD ,&scancode) != 0) {
    printf("Error reading OUT_BUF\n");
    return;
  }
}

int (keyboard_restore)() {
    return 1;
}
