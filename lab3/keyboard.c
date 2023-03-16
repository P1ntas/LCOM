#include <keyboard.h>

/* ------ keyboard.c ------ */
int keyboard_hook_id = 1;
uint8_t scancode;

// subscribe interrupts
int keyboard_subscribe_interrupts(uint8_t *bit_no)
{
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(keyboard_hook_id);
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, 
                          &keyboard_hook_id);
}

// unsubscribe interrupts
int keyboard_unsubscribe_interrupts()
{
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
    
    if (read_KBC_output(KBC_OUT_CMD, &scancode) == 1) {
        printf("ERROR: Could not read scancode!\n");
        return;
    } 
    return;
}

int kbc_restore()
{
  uint8_t commandWord;

  // Leitura da configuração atual
  if (write_KBC_command(KBC_IN_CMD, KBC_READ_CMD) != 0)
    return 1;  // notificar o i8042 da leitura
    
  if (read_KBC_output(KBC_OUT_CMD, &commandWord) != 0)
    return 1;  // ler a configuração

  // Activar o bit das interrupções
  commandWord = commandWord | BIT(0);

  // Escrita da nova configuração
  if (write_KBC_command(KBC_IN_CMD, KBC_OUT_CMD) != 0)
    return 1;  // notificar o i8042 da escrita
    
  if (write_KBC_command(KBC_OUT_CMD, commandWord) != 0)
    return 1;  // escrever a configuração
    
  return 0;
}
