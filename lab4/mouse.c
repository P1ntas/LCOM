#include "mouse.h"

struct packet mouse_byte;
int hook_id_mouse = 3;
uint8_t byte_index = 0;       // [0..2]
uint8_t packet[3];            // pacote
uint8_t current_byte = 0; 

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(hook_id_mouse);

    return sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id_mouse);
}

void (mouse_ih)() {
    if (read_KBC_output(KBC_WRITE_CMD, &current_byte, 1)) printf("Error reading byte\n");
}

void (mouse_sync_bytes)() {
    if (byte_index == 0 && (current_byte & BIT(3))) {
        packet[byte_index] = current_byte;
        byte_index++;
    }
    else if (byte_index > 0) {
        packet[byte_index] = current_byte;
        byte_index++;
    }
}

void (mouse_bytes_to_packet)() {
    for (int i = 0; i < 3; i++)
    {
        mouse_byte.bytes[i] = packet[i];
    }
    mouse_byte.lb = packet[0] & BIT(0);
    mouse_byte.mb = packet[0] & BIT(2);
    mouse_byte.rb = packet[0] & BIT(1);
    mouse_byte.x_ov = packet[0] & BIT(6);
    mouse_byte.y_ov = packet[0] & BIT(7);
    mouse_byte.delta_x = (packet[0] & BIT(4)) ? (0xFF00 | packet[1]) : packet[1];
    mouse_byte.delta_y = (packet[0] & BIT(5)) ? (0xFF00 | packet[2]) : packet[2];
}

int (mouse_write)(uint8_t command) {
    uint8_t attemps = MAX_ATTEMPS;
    uint8_t response;

  do {
    attemps--;
    if (write_KBC_command(KBC_IN_CMD, 0xD4)) return 1;
    if (write_KBC_command(KBC_OUT_CMD, command)) return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (read_KBC_output(KBC_OUT_CMD, &response, 1)) return 1;
    if (response == 0xFA) return 0;
  } while (response != 0xFA && attemps);

  return 1;
}
