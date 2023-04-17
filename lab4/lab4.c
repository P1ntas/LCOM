// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>



#include "i8042.h"
#include "i8254.h"
#include "KBC.h"
#include "mouse.h"
#include "timer.c"

// Any header files included below this line should have been created by you
extern struct packet mouse_byte;
extern uint8_t byte_index;
extern int counter;

typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END
} State;

State state = START;
int16_t delta_x = 0, delta_y = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    
    int ipc_status = 0;
    message msg;
    uint8_t hook_id_mouse = 0;


  if (mouse_subscribe_int(&hook_id_mouse) != 0) return 1;

  if (mouse_write(0xF4) != 0) return 1;

  while (cnt) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 
          if (msg.m_notify.interrupts & hook_id_mouse) { 
            mouse_ih();                   
            mouse_sync_bytes();             
            if (byte_index == 3) {               
              mouse_bytes_to_packet();              
              mouse_print_packet(&mouse_byte);     
              byte_index = 0;
              cnt--;
            }
          }
          break;
      }
    }
  }
  if (mouse_write(0xF5) != 0) return 1;

  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    int ipc_status;
    message msg;
    uint8_t seconds = 0;
    uint8_t hook_id_mouse = 0, hook_id_timer = 0; // Para interpretar as interrupções
    uint16_t timer_freq = sys_hz();

    if (mouse_subscribe_int(&hook_id_mouse) != 0) return 1;
    if (timer_subscribe_int(&hook_id_timer) != 0) return 1;

    if (mouse_write(0xF4) != 0) return 1; // A

    while (seconds < idle_time) { // Só termina quando passarmos @idle_time sem ler pacotes

        if (driver_receive(ANY, &msg, &ipc_status) != 0){
            printf("Error");
            continue;
        }

        if (is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 

                if (msg.m_notify.interrupts & hook_id_timer) { // Se for uma interrupão do timer
                    timer_int_handler();
                    if (counter % timer_freq == 0) seconds++;
                }

                if (msg.m_notify.interrupts & hook_id_mouse) {  
                mouse_ih();                               
                mouse_sync_bytes();                       
                if (byte_index == 3) {                   
                    mouse_bytes_to_packet();           
                    mouse_print_packet(&mouse_byte);      
                    byte_index = 0;
                }
                seconds = 0;
                counter = 0;
                }
            }
        }
    }
    if (mouse_write(0xF5) != 0) return 1;

    if (timer_unsubscribe_int() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;

    return 0;
}

void (state_machine)(uint8_t tolerance, uint8_t x_len) {

    switch (state) {

      case START:
          delta_x = 0;
          delta_y = 0;
          if (mouse_byte.lb && !mouse_byte.rb && !mouse_byte.mb) {
            state = UP;
          }
          break;

      case UP:
          if (!mouse_byte.lb && !mouse_byte.rb && !mouse_byte.mb) {
            if (delta_x >= x_len && (delta_y / delta_x) > 1) {
              delta_x = 0;
              delta_y = 0;
              state = VERTEX;
            } else state = START;
          }
          else if (mouse_byte.lb && !mouse_byte.rb && !mouse_byte.mb) {
            if (mouse_byte.delta_x <= 0 || mouse_byte.delta_y <= 0) {
              if (abs(mouse_byte.delta_x) > tolerance || abs(mouse_byte.delta_y) > tolerance) {
                state = START;
                break;
              }
            }
            if (mouse_byte.delta_x != 0 && (mouse_byte.delta_y / mouse_byte.delta_x) <= 1) {
              state = START;
            }
            else {
              delta_x += mouse_byte.delta_x;
              delta_y += mouse_byte.delta_y;
            }
          }
          
          else state = START;
          break;

      case VERTEX:
          if (!mouse_byte.lb && mouse_byte.rb && !mouse_byte.mb) {
            if (abs(delta_x) > tolerance || abs(delta_y) > tolerance) {
              state = START;
            }
            else {
              delta_x = 0;
              delta_y = 0;
              state = DOWN;
            }
          }
          else if (mouse_byte.lb && !mouse_byte.rb && !mouse_byte.mb) {
            delta_x = 0;
            delta_y = 0;
            state = UP;
          }
          else if (mouse_byte.delta_x != 0 && mouse_byte.delta_y != 0) {
            delta_x += mouse_byte.delta_x;
            delta_y += mouse_byte.delta_y;
          }
          else state = START;
          break;

      case DOWN:
          if (!mouse_byte.lb && !mouse_byte.rb && !mouse_byte.mb) {
            if ((delta_x = x_len) && (delta_y / delta_x) < -1) {
              state = END;
              break;
            }
            else state = START;
          }
          else if (mouse_byte.delta_x != 0 && mouse_byte.delta_y != 0) {
            if (mouse_byte.delta_x <= 0 || mouse_byte.delta_y >= 0) {
              if (abs(mouse_byte.delta_x) > tolerance || abs(mouse_byte.delta_y) > tolerance) {
                state = START;
                break;
              }
            }
            if (mouse_byte.delta_x != 0 && (mouse_byte.delta_y / mouse_byte.delta_x) >= -1) {
              state = START;
            }
            else {
              delta_x += mouse_byte.delta_x;
              delta_y += mouse_byte.delta_y;
            }
          }
          else state = START;
          break;

      case END:
          break;
      default:
        break;
    }
}


int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    
    int ipc_status;
    message msg;
    uint8_t hook_id_mouse;

    if (mouse_subscribe_int(&hook_id_mouse) != 0) return 1;

    if (mouse_write(0xF4) != 0) return 1;

    while (state != END) { 

        if (driver_receive(ANY, &msg, &ipc_status) != 0){
        printf("Error");
        continue;
        }

        if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 

            if (msg.m_notify.interrupts & hook_id_mouse) { 
                mouse_ih();                               
                mouse_sync_bytes();                      
                if (byte_index == 3) {                    
                  mouse_bytes_to_packet();               
                  state_machine(tolerance, x_len);        
                  byte_index = 0;
                }
            }
        }
        }
  }

  if (mouse_write(0xF5) != 0) return 1;

  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    
    while (cnt) { // Só termina quando lermos @cnt pacotes

        if (mouse_write(0xEB) != 0) return 1;  // Comando para lermos diretamente mais um byte
        mouse_ih();                                       // Lemos mais um byte    
        mouse_sync_bytes();                               // Sincronizamos esse byte no pacote respectivo      
        if (byte_index == 3) {                            // Quando tivermos três bytes do mesmo pacote
            mouse_bytes_to_packet();                      // Formamos o pacote
            mouse_print_packet(&mouse_byte);            // Mostramos o pacote 
            byte_index = 0;
            cnt--;
            tickdelay(micros_to_ticks(period * 1000));    // Esperamos @period microssegundos até ler o próximo pacote
        }
    }

    // Ativar o Stream Mode
    if (mouse_write(0xEA) != 0) return 1;

    // Desativar o report de dados do rato
    if (mouse_write(0xF5) != 0) return 1;
  
    // KBC volta ao estado normal
    uint8_t commandByte = minix_get_dflt_kbc_cmd_byte();              // buscar o comando adequado, já dado pela LCF
    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD) != 0) return 1;  // avisar o i8042 para receber o próximo comando
    if (write_KBC_command(KBC_WRITE_CMD, commandByte) != 0) return 1; // enviar o comando
  
    return 0;
}
