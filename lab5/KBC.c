#include <KBC.h>

int (read_KBC_status)(uint8_t* status) {

    return util_sys_inb(KBC_STATUS_REG, status);
}

int read_KBC_output(uint8_t port, uint8_t *output, uint8_t mouse)
{
  uint8_t status;
  uint8_t attemps = 10;
    
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
      
      if((status & BIT(7)) != 0) {
        // verifica erro de paridade
        printf("Error: Parity error!\n");
        return 1;
      }
      
      if ((status & BIT(6)) != 0) {
        // verifica erro de timeout
        printf("Error: Timeout error!\n");
        return 1;
      }
      if (mouse && !(status & BIT(5))) {              // está à espera do output do rato
        printf("Error: Mouse output not found\n");  // mas o output não é do rato
        return 1;
      } 
      if (!mouse && (status & BIT(5))) {                 // está à espera do output do teclado
        printf("Error: Keyboard output not found\n"); // mas o output não é do teclado
        return 1;
      } 
      return 0;
    }
   
    tickdelay(micros_to_ticks(20000));
    attemps--;
  }
  printf("wrong\n");
  return 1; // se ultrapassar o número de tentativas lança um erro
}

int write_KBC_command(uint8_t port, uint8_t commandByte)
{
  uint8_t status;
  uint8_t attemps = 10;

  while (attemps) {
    if (read_KBC_status(&status) != 0) { // ler o status
      printf("Error: Status not available!\n");
      return 1;
    }

    if ((status & BIT(1)) == 0) {
      // o input buffer não está cheio, pode-se escrever
      if (sys_outb(port, commandByte) != 0) {
        // tentativa de escrita
        printf("Error: Could not write commandByte!\n");
        return 1;
      }
      
      return 0; // sucesso: comando inserido no i8042
    }
    
    tickdelay(micros_to_ticks(20000));              
    attemps--;
  }
  
  return 1; // se ultrapassar o número de tentativas lança um erro
}
