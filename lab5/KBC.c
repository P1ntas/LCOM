#include <lcom/lcf.h>
#include "KBC.h"

#include <stdint.h>

#include "i8254.h"
#include "i8042.h"

int no_calls = 0;

int (read_KBC_status)(uint8_t *status)
{
    if (util_sys_inb(KBC_STATUS_REG, status))
    {
        printf("Error: Could not get current status!\n");
        return TRUE;
    }

    no_calls++;

    printf("%s is implemented!\n", __func__);

    return FALSE;
}

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse)
{
    uint8_t status = 0;                           // número de tentativas seguidas antes de terminar por erro.
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts)
    {
        if (read_KBC_status(&status))                              // ler o status
        { 
            printf("Error: Status not available!\n");
            return TRUE;
        }
        
        printf("Status: %x \n", status);

        if ((status & FULL_OUT_BUFFER))                            // o output buffer está cheio, pode-se ler BIT(0)
        {
            if (util_sys_inb(port, output))                        // leitura do buffer de saída
            {
                printf("Error: Could not read output!\n");
                return TRUE;
            }
            no_calls++;
        
            if((status & PARITY_ERROR))                            // verifica erro de paridade BIT(7)
            {
                printf("Error: Parity error!\n");
                return TRUE;
            }
        
            if ((status & TIMEOUT_ERROR))                          // verifica erro de timeout BIT(6)
            {
                printf("Error: Timeout error!\n");
                return TRUE;
            }
            if (mouse && !(status & BIT(5))) 
            {
                printf("Error: Mouse output not found\n");
                return TRUE;
            } 
            if (!mouse && (status & BIT(5))) 
            {
                printf("Error: Keyboard output not found\n");
                return TRUE;
            }
        
            return FALSE;
        }

        printf("Attempts checking buffer full: %d \n", attempts);
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
        
    }   


    

    printf("Error: Could not read KBC output after %d attempts!\n", MAX_ATTEMPTS);
 
    return TRUE;                                                   // se ultrapassar o número de tentativas lança um erro
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte)
{
    uint8_t status = 0;                          // 10
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts)
    {
        if (read_KBC_status(&status))                              // ler o status
        { 
            printf("Error: Status not available!\n");
            return TRUE;
        }
        if (!(status & FULL_IN_BUFFER)) 
        {
            printf("The buffer is not full, can write!\n");
            // o input buffer não está cheio, pode-se escrever
            if (sys_outb(port, commandByte)) 
            {
                // tentativa de escrita
                printf("Error: Could not write commandByte!\n");
                return TRUE;
            }
        
            return FALSE;                                          // sucesso: comando inserido no i8042
        }
    
        printf("Attempts checking buffer full: %d", attempts);
        tickdelay(micros_to_ticks(WAIT_KBC));   
        attempts--;           
    }


    printf("Error: Could not write KBC command after %d attempts!\n", MAX_ATTEMPTS);
    
    return TRUE;                                                  // se ultrapassar o número de tentativas lança um erro
}

