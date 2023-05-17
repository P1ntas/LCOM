#include <lcom/lcf.h>
#include "keyboard.h"

#include <stdint.h>

#include "i8254.h"
#include "i8042.h"

uint8_t scancode = 0;
int hook_id_keyboard =  IRQ_KEYBOARD;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no)
{
    if(bit_no == NULL)
    {
        printf("Error: bit_no is NULL? -> %x!\n", bit_no); 
        return TRUE;
    }

    *bit_no = BIT(hook_id_keyboard);

    if(sys_irqsetpolicy(IRQ_KEYBOARD,IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard))
    {
        printf("Error: Something went wrong while setting IRQ policy!\n");
        return TRUE;
    }

    printf("%s is implemented!\n", __func__);
    
    return FALSE;
}

int (keyboard_unsubscribe_interrupts)()
{
    if(sys_irqrmpolicy(&hook_id_keyboard))
    {
        printf("Error: Something went wrong while unsubscribing a previous subscription of the interrupt notification associated with the specified hook_id!\n");
        return TRUE;
    }

    printf("%s is implemented!\n", __func__);

    return FALSE;
}

void (kbc_ih)()
{
    if(read_KBC_output(KBC_OUT_CMD, &scancode, 0))
    {
        printf("Error: Could not read scancode!\n");
    }
    
    printf("%s is implemented, scancode: %d!\n", __func__, scancode);
}

int keyboard_restore()
{
    uint8_t commandWord;

    // Leitura da configuração atual
    if(write_KBC_command(KBC_IN_CMD, KBC_READ_CMD))
    {
        printf("Error: Could not notify KBC (i8042): wanted to read current configuration (command not send to input buffer)!\n");
        return TRUE;  // notificar o i8042 da leitura
    }
    
    if(read_KBC_output(KBC_OUT_CMD, &commandWord, 0))
    {
        printf("Error: Could not read current keyboard configuration from the output buffer!\n");
        return TRUE;  // ler a configuração
    }

    // Activar o bit das interrupções
    commandWord = commandWord | ENABLE_INT;

    // Escrita da nova configuração
    if(write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD))
    {
        printf("Error: Could not notify KBC (i8042): wanted to write a new configuration (command not send to input buffer)!\n");
        return TRUE;  // notificar o i8042 da escrita
    }
    
    if(write_KBC_command(KBC_OUT_CMD, commandWord))
    {
        printf("Error: Could not write new keyboard configuration to the output buffer!\n");
        return TRUE;  // escrever a configuração       
    }

    printf("%s is implemented!\n", __func__);   
    
    return FALSE;
}
