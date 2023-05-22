#include "keyboard.h"

uint8_t scancode = 0;
int keyboard_hook_id = 1;

int (keyboard_subscribe_interrupts)() {
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_interrupts)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
    read_KBC_output(KBC_OUT_CMD, &scancode, 0);
}

int (keyboard_restore)() {
    
    uint8_t commandWord;

  if (write_KBC_command(KBC_IN_CMD, KBC_READ_CMD) != 0) return 1; 
    
  if (read_KBC_output(KBC_OUT_CMD, &commandWord, 0) != 0) return 1;

  commandWord = commandWord | BIT(0);

  if (write_KBC_command(KBC_IN_CMD, KBC_OUT_CMD) != 0) return 1;
    
  if (write_KBC_command(KBC_OUT_CMD, commandWord) != 0) return 1;
    
  return 0;
}
