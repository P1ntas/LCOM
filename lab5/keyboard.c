#include<lcom/lcf.h>

#include <stdint.h>

#include "keyboard.h"


/* ------ keyboard.c ------ */
int keyboard_hook_id = 1/*IRQ_KEYBOARD*/;
uint8_t scancode = 0;

// subscribe interrupts
int keyboard_subscribe_interrupts (uint8_t *bit_no)
{
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(keyboard_hook_id);
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(1/*IRQ_KEYBOARD*/, IRQ_REENABLE | IRQ_EXCLUSIVE, 
                          &keyboard_hook_id);
}

// unsubscribe interrupts
int keyboard_unsubscribe_interrupts ()
{
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
  if (read_KBC_output(0x60/*KBC_OUT_CMD*/,&scancode) != 0) {
    printf("Error reading OUT_BUF\n");
    return;
  }
}

int (keyboard_restore)() {
  unsigned char cmd;

  //notify
  if (write_KBC_command( 0x64/*KBC_IN_CMD*/, 0x20/*KBC_READ_CMD*/) != 0) {
    printf("Error writing command to KBC_IN_CMD\n");
    return 1;
  }
  //read
  if (read_KBC_output(0x60/*KBC_OUT_CMD*/, &cmd) != 0) {
    printf("Error reading OUT_BUF\n");
    return 1;
  }
  //prepare cmd
  cmd = cmd & 1/*ENABLE_INT*/;
  //notify
  if (write_KBC_command(0x64/*KBC_IN_CMD*/, 0x60/*KBC_WRITE_CMD*/) != 0) {
    printf("Error writing command to KBC_IN_CMD\n");
    return 1;
  }
  //write
  if (write_KBC_command(0x60/*KBC_OUT_CMD*/, cmd) != 0) {
    printf("Error writing command to KBC_OUT_CMD\n");
    return 1;
  }
  //success
  return 0;
}
