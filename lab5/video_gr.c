#include <lcom/lcf.h>
#include <stdio.h>

static vbe_mode_info_t info;
static void *video_mem;  /* frame-buffer VM address */
static uint16_t hres = 0;    /* XResolution */
static uint16_t vres = 0;    /* YResolution */
static unsigned bytes_per_pixel = 0;

void *(vg_init) (uint16_t mode) 
{
  /* Address to which VRAM is mapped */
  void *ptr = NULL;
  
  if(vbe_get_mode_info(mode, &info))
  {
    return ptr;
  }
    
  hres = info.XResolution;
  vres = info.YResolution;

  int r;
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_base; /* VRAM’s physical addresss */
  unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */

  bytes_per_pixel = (info.BitsPerPixel+7) >> 3;

  vram_base = info.PhysBasePtr;
  vram_size = hres * vres * bytes_per_pixel;

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size; 

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) 
  {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return ptr;  
  }

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED) 
  {
    panic("couldn’t map video memory");
    return ptr;
  }

  struct reg86 r86;

  memset(&r86, 0, sizeof(r86));
  
  // r.ah = 0x4F;
  // r.al = 0x02;  
  r86.intno = 0x10;
  r86.ax = 0x4F02;      // VBE call, function 02 -- set VBE mode :  r.ah = 0x4F;    /* Set Video Mode function */ + r.al = 0x02;    /* 80x25 graphics mode*/        
  r86.bx = mode | BIT(14);
  

  if (sys_int86(&r86) != OK) 
  {
    printf("set_vbe_mode: sys_int86() failed \n");
    return ptr;
  }

  return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
  if (x >= hres || y >= vres)
  {
    printf("Error: Out of bounds!\n");
    return TRUE;
  }

  // for (uint16_t i = 0; i < len; i++)
  // {
  //   uint8_t * ptr;

  //   ptr = (uint8_t*)video_mem + ((x+i) * bytes_per_pixel) + (y * hres * bytes_per_pixel);

  //   uint8_t temp;

  //   for (unsigned int j = 0; j < bytes_per_pixel; j++) 
  //   {
  //     temp = color & 0xFF;
  //     *(ptr + j) = temp;
  //     color = color >> 8;
  //   }
  // }

  // for(uint16_t column = 0; column < len; column++)
  // {
  //   memcpy((char *) malloc(hres * vres * bytes_per_pixel) + (y * hres + (x + column)) * bytes_per_pixel, &color, bytes_per_pixel);
  // }
  
  

  // char *ptr = video_mem;
  // ptr += (hres * y + x) * (info.BitsPerPixel / 8); 
  // *ptr = color;

  // for(int i = 0 ; i < len ; i++) {
  //   ptr += (info.BitsPerPixel / 8);
  //   *ptr = color;
  // }

  return FALSE;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
  for (uint16_t line = 0; line < height; line++)
  {
    if (vg_draw_hline(x, y + line, width, color))
    {
      printf("Error: something went wrong while drawing the line!\n");
      return TRUE;
    }
  }
  

  return FALSE;
}

uint16_t get_BitsPerPixel() 
{
  return info.BitsPerPixel;
}

uint16_t get_RedMaskSize() 
{
  return info.RedMaskSize;
}

uint16_t get_GreenMaskSize() 
{
  return info.GreenMaskSize;
}
uint16_t get_BlueMaskSize() 
{
  return info.BlueMaskSize;
}

uint8_t get_color_R(uint16_t mode, uint32_t color) 
{
  unsigned int R_mask;

  switch (mode) 
  {
    case 0x110: // (1:)5:5:5
      R_mask = 0x7C00;
      return ((color & R_mask) >> 10);
    case 0x115: // 8:8:8
      R_mask = 0xFF0000;
      return ((color & R_mask) >> 16);
    case 0x11A: // 5:6:5
      R_mask = 0xF800;
      return ((color & R_mask) >> 11);
    case 0x14C: // (8:)8:8:8
      R_mask = 0xFF0000;
      return ((color & R_mask) >> 16);
    default:
      printf("Error: Something went wrong while getting Red color! \n");
      return TRUE;
  }
  return FALSE;
}

uint8_t get_color_G(uint16_t mode, uint32_t color) 
{
  unsigned int G_mask;

  switch (mode) 
  {
    case 0x110:
      G_mask = 0x3E0;
      return ((color & G_mask) >> 5);
    case 0x115:
      G_mask = 0x00FF00;
      return ((color & G_mask) >> 8);
    case 0x11A:
      G_mask = 0x7E0;
      return ((color & G_mask) >> 5);
    case 0x14C:
      G_mask = 0x00FF00;
      return ((color & G_mask) >> 8);
    default:
      printf("Error: Something went wrong while getting Green color! \n");
      return TRUE;
  }

  return FALSE;
}

uint8_t get_color_B(uint16_t mode, uint32_t color) 
{
  unsigned int B_mask;

  switch (mode) 
  {
    case 0x110:
      B_mask = 0x1F;
      return (color & B_mask);
    case 0x115:
      B_mask = 0xFF;
      return (color & B_mask);
    case 0x11A:
      B_mask = 0x1F;
      return (color & B_mask);
    case 0x14C:
      B_mask = 0xFF;
      return (color & B_mask);
    default:
      printf("Error: Something went wrong while getting Blue color! \n");
      return TRUE;
  }
  return 1;
}

uint32_t combineRGB(uint16_t mode, uint8_t R, uint8_t G, uint8_t B) 
{
  uint32_t color;

  switch (mode) 
  {
    case 0x110: // (1:)5:5:5
      color = ((R<<10)|(G<<5)|(B));
      return color;
    case 0x115: // 8:8:8
      color = ((R<<16)|(G<<8)|(B));
      return color;
    case 0x11A: // 5:6:5
      color = ((R<<11)|(G<<5)|(B));
      return color;
    case 0x14C: // (8:)8:8:8
      color = ((R<<16)|(G<<8)|(B));
      return color;
    default:
      printf("Error: Something went wrong while getting combining using RGB method! \n");
      return TRUE;
  }
  return 1;
}


