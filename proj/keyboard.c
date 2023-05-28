#include "keyboard.h"

uint8_t scancode = 0;
int hook_id_keyboard =  IRQ_KEYBOARD;

int (keyboard_subscribe_int)(/*uint8_t *bit_no*/)
{

    if(sys_irqsetpolicy(IRQ_KEYBOARD,IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard)) return TRUE;
    
    return FALSE;
}

int (keyboard_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&hook_id_keyboard)) return TRUE;

    return FALSE;
}

int aux_keyboard_read(uint8_t port, uint8_t *output) {
    uint8_t status;
    uint8_t attemps = 10;
    
    while (attemps) {

        if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {             
            return 1;
        }

        if ((status & BIT(0)) != 0) {                   
            if (util_sys_inb(port, output) != 0) {          
                return 1;
            }
            if((status & BIT(7)) != 0) return 1;

            if((status & BIT(6)) != 0) return 1;

            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attemps--;
    }
    return 1; 
}

void (kbc_ih)()
{
    aux_keyboard_read(KBC_OUT_CMD, &scancode);
}

int aux_keyboard_write(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t attemps = 10;

    while (attemps) {

        if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

        if ((status & BIT(1)) == 0) {

            if(sys_outb(port, commandByte) != 0) {
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attemps--;
    }
    return 1;
}

int (keyboard_restore)()
{
    uint8_t commandWord;

    if(aux_keyboard_write(KBC_IN_CMD, KBC_READ_CMD))
    {
        return TRUE;  // notificar o i8042 da leitura
    }
    
    if(aux_keyboard_read(KBC_OUT_CMD, &commandWord))
    {
        return TRUE;  // ler a configuração
    }

    commandWord = commandWord | ENABLE_INT;

    if(aux_keyboard_write(KBC_IN_CMD, KBC_WRITE_CMD)) return TRUE;  // notificar o i8042 da escrita
    
    if(aux_keyboard_write(KBC_OUT_CMD, commandWord)) return TRUE;  // escrever a configuração       
    
    return FALSE;
}
