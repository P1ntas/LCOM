#include <lcom/lcf.h>
#include <stdint.h>

#include "KBC.h"

int no_sys_call = 0;

int(read_KBC_status)(uint8_t *status) {
  if (status == NULL) return 1;

  if (util_sys_inb(0x64, status) != 0) return 1;

  no_sys_call++;
  return 0;
}

int(read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse) {
  uint8_t status;
  uint8_t attemps = 10;

  while (attemps) {
    // 1. ler o status register
    if (read_KBC_status(&status)) { // ler o status
      printf("Error: Status not available!\n");
      return 1;
    }
    // 2. verificar se o bit 0 está a 0 (é possível escrever)E
    if ((status & BIT(0))) {
      // o output buffer está cheio, pode-se ler
      if (util_sys_inb(port, output)) {
        // leitura do buffer de saída
        printf("Error: Could not read output!\n");
        return 1;
      }
      no_sys_call++;
      // 2.1 Verificar o erro de paridade
      if (mouse && (status & BIT(7))) return 1;
      // 2.2 Verificar o erro de timeout
      if (mouse && (status & BIT(6))) return 1;
      // O output do rato
      if (mouse && !(status & BIT(5))) return 1;
      // O output do teclado
      if (!mouse && (status & BIT(5))) return 1;
    
      return 0;
    }

    tickdelay(micros_to_ticks(20000));
    attemps--;
  }
  // se ultrapassar o número de tentativas lança um erro
  return 1; 
}

int(write_KBC_command)(uint8_t port, uint8_t commandByte) {
  uint8_t status;
  uint8_t attemps = 10;

  while (attemps) {
    if (read_KBC_status(&status)) return 1;
    
    if ((status & BIT(1)) == 0) {
      // o input buffer não está cheio, pode-se escrever
      if (sys_outb(port, commandByte)) return 1;
  
      return 0; // sucesso: comando inserido no i8042
    }

    tickdelay(micros_to_ticks(20000));
    attemps--;
  }

  return 1; // se ultrapassar o número de tentativas lança um erro
}

int write_to_mouse(uint8_t command)
{
  uint8_t attemps = 10;
  uint8_t mouse_response = 0;

  // Enquanto houver tentativas e a resposta não for satisfatória
  do {
    attemps--;
    // Ativar do modo D4 do i8042
    if (write_KBC_command(0x64, 0xD4))
      return 1;
    // O comando para o rato é escrito na porta 0x60
    if (write_KBC_command(0x60, command))
      return 1;
    tickdelay(micros_to_ticks(20000));
    // Ler a resposta do rato pela porta 0x60
    if (util_sys_inb(0x60, &mouse_response))
      return 1;
    // Se a resposta for 0xFE, tentar disab
    if (mouse_response == 0xFE) {
      tickdelay(micros_to_ticks(20000));
      if (write_to_mouse(0xF5))
        return 1;
      tickdelay(micros_to_ticks(20000));
      if (write_to_mouse(0xF4))
        return 1;
    }
    
  } while (mouse_response != 0xFA && attemps);       

  return 0;
}
