#include <lcom/lcf.h>

#include <stdint.h>

#include "KBC.h"
#include "i8042.h"


int no_sys_call = 0;

int(read_KBC_status)(uint8_t *status) {
  if (status == NULL) return 1;

  if (util_sys_inb(KBC_STATUS_REG, status) != 0) return 1;

  no_sys_call++;
  return 0;
}

int(read_KBC_output)(uint8_t port, uint8_t *output) {
  uint8_t status;
  uint8_t attemps = MAX_ATTEMPS;

  while (attemps) {
    if (read_KBC_status(&status) != 0) { // ler o status
      printf("Error: Status not available!\n");
      return 1;
    }
    
    if ((status & BIT(0)) != 0) {
      // o output buffer está cheio, pode-se ler
      if (util_sys_inb(port, output) != 0) {
        // leitura do buffer de saída
        printf("Error: Could not read output!\n");
        return 1;
      }

      no_sys_call++;

      if ((status & BIT(7)) != 0) {
        // verifica erro de paridade
        printf("Error: Parity error!\n");
        return 1;
      }

      if ((status & BIT(6)) != 0) {
        // verifica erro de timeout
        printf("Error: Timeout error!\n");
        return 1;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(WAIT_KBC));
    attemps--;
  }

  return 1; // se ultrapassar o número de tentativas lança um erro
}

int(write_KBC_command)(uint8_t port, uint8_t commandByte) {
  uint8_t status;
  uint8_t attemps = MAX_ATTEMPS;

  while (attemps) {
    if (read_KBC_status(&status)) { // ler o status
      printf("Error: Status not available!\n");
      return 1;
    }

    if ((status & BIT(1)) == 0) {
      // o input buffer não está cheio, pode-se escrever
      if (sys_outb(port, commandByte)) {
        // tentativa de escrita
        printf("Error: Could not write commandByte!\n");
        return 1;
      }

      return 0; // sucesso: comando inserido no i8042
    }

    tickdelay(micros_to_ticks(WAIT_KBC));
    attemps--;
  }

  return 1; // se ultrapassar o número de tentativas lança um erro
}