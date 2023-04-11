#include <lcom/lcf.h>
#include "mouse.h"

#include <stdint.h>

#include "i8254.h"
#include "i8042.h"



uint8_t current_byte = 0;
uint8_t packet[3];
uint8_t byte_index = 0;

struct packet mouse_packet;


int hook_id_keyboard =  IRQ_KEYBOARD;
int hook_id_mouse = IRQ_MOUSE;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no)
{
    if(bit_no == NULL)
    {
        printf("Error: bit_no is NULL? -> %x!\n", bit_no); 
        return TRUE;
    }

    *bit_no = BIT(hook_id_keyboard);

    if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard))
    {
        printf("Error: Something went wrong while setting IRQ policy (keyboard)!\n");
        return TRUE;
    }

    printf("%s is implemented!\n", __func__);
    
    return FALSE;
}

int (mouse_subscribe_interrupts)(uint8_t *bit_no)
{
    if(bit_no == NULL)
    {
        printf("Error: bit_no is NULL? -> %x!\n", bit_no); 
        return TRUE;
    }

    *bit_no = BIT(hook_id_mouse);

    if(sys_irqsetpolicy(IRQ_LINE_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse))
    {
        printf("Error: Something went wrong while setting IRQ policy (mouse)!\n");
        return TRUE;
    }

    printf("%s is implemented!\n", __func__);
    
    return FALSE;
}

int (keyboard_unsubscribe_interrupts)()
{
    if(sys_irqrmpolicy(&hook_id_keyboard))
    {
        printf("Error: Something went wrong while unsubscribing a previous subscription of the interrupt notification associated with the specified hook_id (keyboard)!\n");
        return TRUE;
    }

    printf("%s is implemented!\n", __func__);

    return FALSE;
}

int (mouse_unsubscribe_interrupts)()
{
    if(sys_irqrmpolicy(&hook_id_mouse))
    {
        printf("Error: Something went wrong while unsubscribing a previous subscription of the interrupt notification associated with the specified hook_id (mouse)!\n");
        return TRUE;
    }

    printf("%s is implemented!\n", __func__);

    return FALSE;
}

void (kbc_ih)()
{
    if(read_KBC_output(KBC_OUT_CMD, &current_byte, 0))
    {
        printf("Error: Could not read scancode (current byte also)!\n");
    }
    
    printf("%s is implemented, scancode: %d!\n", __func__, current_byte);
}

void (mouse_ih)() 
{
    if (read_KBC_output(KBC_OUT_CMD, &current_byte, 1))
    {
        printf("Error: Could not read current byte!\n");
    }

    printf("%s is implemented, current byte: %d!\n", __func__, current_byte);
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

int write_to_mouse(uint8_t command)
{
  uint8_t attempts = 10;
  uint8_t mouse_response = 0;

  // Enquanto houver tentativas e a resposta não for satisfatória
  do 
  {
    
    printf("Attempts: %d \n", attempts);
    // Ativar do modo D4 do i8042
    if (write_KBC_command(KBC_IN_CMD, FOWARD_REQ))
    {
        printf("Error: Could not request forwarding of byte (command) to the mouse!\n");
        return TRUE;
    }    
    
    // O comando para o rato é escrito na porta 0x60
    if (write_KBC_command(KBC_OUT_CMD, command))
    {
        printf("Error: Could not write the byte (command)!\n");
        return TRUE;
    }

    tickdelay(micros_to_ticks(WAIT_KBC));
            
    // Ler a resposta do rato pela porta 0x60
    if (util_sys_inb(KBC_OUT_CMD, &mouse_response))
    {
        printf("Error: Could not get current status!\n");
        return TRUE;
    }

    if(mouse_response == NACK)
    {
        tickdelay(micros_to_ticks(WAIT_KBC));
        if (write_to_mouse(DISABLE_DEV))
        {
            printf("Error: Could not disable the device (the host should disable the device with a Disable (0xF5) command before sending any other command)!\n");
            return TRUE;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        if (write_to_mouse(ENABLE_DEV))
        {
            printf("Error: Could not enable the device (If the device is in Stream mode (the default) and has been enabled with an Enable (0xF4) command)!\n");
            return TRUE;
        }
    }
  } 
  while (mouse_response != ACK && attempts);       

  return FALSE;
}

void (mouse_sync_bytes)()
{
    if (byte_index == 0 && (current_byte & CONTROL)) 
    {
        packet[byte_index] = current_byte;
        byte_index++;
    }
    else if (byte_index > 0) 
    {
        packet[byte_index] = current_byte;
        byte_index++;
    }
}

void (mouse_parse_packet)()
{
    for (int i = 0 ; i < 3 ; i++) 
    {
        mouse_packet.bytes[i] = packet[i];
    }

    mouse_packet.lb = packet[0] & PRESSED_LB;
    mouse_packet.rb = packet[0] & PRESSED_RB;
    mouse_packet.mb = packet[0] & PRESSED_MB;
    mouse_packet.delta_x = (packet[0] & DELTA_X) ? ( 0xFF00 | packet[1] ) : packet[1];
    mouse_packet.delta_y = (packet[0] & DELTA_Y) ? ( 0XFF00 | packet[2] ) : packet[2];
    mouse_packet.x_ov = packet[0] & OVERFLOW_X;
    mouse_packet.y_ov = packet[0] & OVERFLOW_Y;
}

