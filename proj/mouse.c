#include "mouse.h"

int hook_id_mouse = 2; 
uint8_t byte_index = 0;
uint8_t current_byte; 
uint8_t mouse_bytes[3]; 
mouse_info_t mouse_info = {0, 0, 100, 100};
extern vbe_mode_info_t mode_info;

int (mouse_subscribe_int)(){
    
    return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);

}

int (mouse_unsubscribe_int)(){
  return sys_irqrmpolicy(&hook_id_mouse);
}

int aux_mouse_read(uint8_t port, uint8_t *output, uint8_t mouse) {

  uint8_t status;
  uint8_t attemps = 10;
    
    while (attemps) {
        if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

        if ((status & BIT(0)) != 0) {                   
            if (util_sys_inb(port, output) != 0) {          
                return 1;
            }
            if((status & BIT(7)) != 0) return 1;

            if((status & BIT(6)) != 0) return 1;

            if (mouse && !(status & BIT(5))) return 1;

            if (!mouse && (status & BIT(5))) return 1;

            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attemps--;
    }
    return 1; 
}

void (mouse_ih)(){
  aux_mouse_read(KBC_WRITE_CMD, &current_byte, 1);
}

void mouse_sync_bytes() {
  if (byte_index == 0 && (current_byte & FIRST_BYTE)) { // é o byte CONTROL, o bit 3 está ativo
    mouse_bytes[byte_index]= current_byte;
    byte_index++;
  }
  else if (byte_index > 0) {                            // recebe os deslocamentos em X e Y
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
}

void (mouse_sync_info)(){

  mouse_info.right_click = mouse_bytes[0] & MOUSE_RB;
  mouse_info.left_click = mouse_bytes[0] & MOUSE_LB;

  if (mouse_bytes[0] & MOUSE_X_OVERFLOW || mouse_bytes[0] & MOUSE_Y_OVERFLOW) return;

  int16_t delta_x = mouse_info.x + ((mouse_bytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1]);
  int16_t delta_y = mouse_info.y - ((mouse_bytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2]);

  if (delta_x < 0 || delta_x > mode_info.XResolution || delta_y < 0 || delta_y > mode_info.YResolution) return;
  mouse_info.x = delta_x;
  mouse_info.y = delta_y;
}

int aux_mouse_write(uint8_t port, uint8_t commandByte) {
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

int (mouse_write)(uint8_t command) {

  uint8_t attemps = 10;
  uint8_t mouse_response;

  do {
    attemps--;
    if (aux_mouse_write(KBC_IN_CMD, WRITE_BYTE_MOUSE)) return 1;
    if (aux_mouse_write(KBC_OUT_CMD, command)) return 1;
    tickdelay(micros_to_ticks(20000));
    if (util_sys_inb(KBC_OUT_CMD, &mouse_response)) return 1;
  } while (mouse_response != ACK && attemps);

  return 0;
}
