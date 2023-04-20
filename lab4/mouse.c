#include <lcom/lcf.h>
#include <stdint.h>

#include "mouse.h"
#include "KBC.h"

int byte_index = 0;
uint8_t mouse_bytes[3];
uint8_t current_byte = 0;

struct packet mouse_packet;

int mouse_hook_id = 2;

extern 

void (mouse_ih)() {
    read_KBC_output(0xD4, &current_byte, 1);
    mouse_sync_bytes();
}

// subscribe interrupts
int (mouse_subscribe_interrupts) (uint8_t *bit_no)
{
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(mouse_hook_id);
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(12, 0x001 | 0x002, 
                          &mouse_hook_id);
}

// unsubscribe interrupts
int (mouse_unsubscribe_interrupts) ()
{
  return sys_irqrmpolicy(&mouse_hook_id);
}


void (load_packet)(struct packet *packet, uint8_t bytes[])
{
  packet->bytes[0] = bytes[0];
  packet->bytes[1] = bytes[1];
  packet->bytes[2] = bytes[2];

  packet->lb = bytes[0] & BIT(0);
  packet->rb = bytes[0] & BIT(1);
  packet->mb = bytes[0] & BIT(2);

  packet->x_ov = bytes[0] & BIT(6);
  packet->y_ov = bytes[0] & BIT(7);

  packet->delta_x = bytes[1];
  packet->delta_y = bytes[2];
  if (packet->delta_x & BIT(4))
    packet->delta_x |= 0xFF00;
  if (packet->delta_y & BIT(4))
    packet->delta_y |= 0xFF00;
}


void (mouse_sync_bytes)()
{
  if (byte_index == 0 && (current_byte & BIT(3))) {
    // é o byte CONTROL, o bit 3 está ativo
    mouse_bytes[byte_index]= current_byte;
    byte_index++;
  }
  if (byte_index == 3) {
    // completou o pacote
    load_packet(&mouse_packet, mouse_bytes);
    mouse_print_packet(&mouse_packet);
    byte_index = 0;
  }
  if (byte_index > 0) {
    // recebe os deslocamentos em X e Y
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
}


