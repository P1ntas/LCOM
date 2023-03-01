#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) 
{  
  if(lsb != NULL)
  {
    *lsb = (uint8_t) val;
    printf("%s is implemented!\n", __func__);
    printf("The lsb is %c!\n", lsb);
    return FALSE;
  }

  printf("Error: lsb is NULL? -> %c!\n", lsb);

  return TRUE;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) 
{  
  if(msb != NULL)
  {
    *msb = (uint8_t)(val >> 8);
    printf("%s is implemented!\n", __func__);
    printf("The msb is %c!\n", msb);
    return FALSE;
  }

  printf("Error: msb is NULL? -> %c!\n", msb); 

  return TRUE;
}

int (util_sys_inb)(int port, uint8_t *value) 
{
  uint32_t val = 0;

  if(value != NULL)
  {
    if(sys_inb(port, &val))
    {
      printf("Error: something happened while calling sys_inb!\n");
      return TRUE;
    }
    *value = (uint8_t) val;
    printf("%s is implemented!\n", __func__);
    printf("The value is %x and port is %x!\n", value, port);
    return FALSE;
  }

  printf("Error: value is NULL? -> %x!\n", value); 

  return TRUE;
}
