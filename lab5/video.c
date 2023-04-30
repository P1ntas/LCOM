// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "video.h"

extern vbe_mode_info_t mode_info;
void *video_mem;

void *(vg_init) (uint16_t mode) {

  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    printf("Failed at getting mode info\n.");
    return NULL;
  }

  /* Lowest memory address in range */
  unsigned int vram_base = mode_info.PhysBasePtr;
  // rounding bytes by excess ((bit+7)/8)
  unsigned int vram_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

  struct minix_mem_range mr;
  mr.mr_base = vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int fail;
  fail = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
  if (fail) {
    printf("sys_privtl failed\n");
    return NULL;
  }

  // mapping the vm to the address of process
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) {
    printf("Couldn't map vid mem.\n");
    return NULL;
  }

  // setting the video mode
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  //r86.ax = 0x4F02;
  r86.bx = BIT(14) | mode;

  fail = sys_int86(&r86);
  if (fail) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }

  return video_mem;
}

int(set_frame_buffer)(uint16_t mode) {

  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    printf("Failed at getting mode info.\n");
    return 1;
  }

  uint16_t vram_base = mode_info.PhysBasePtr;
  uint16_t vram_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

  struct minix_mem_range mr;
  mr.mr_base = vram_base;
  mr.mr_limit = vram_base + vram_size;

  int fail;
  fail = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
  if (fail) {
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) {
    // failed
    return 1;
  }

  return 0;
}

int(set_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  //r86.ax = 0x4F02;
  r86.bx = mode | BIT(14);

  if (sys_int86(&r86) != 0) {
    //failed
    return 1;
  }

  return 0;
}



int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > mode_info.XResolution || y > mode_info.YResolution)
    return 1;

  uint8_t *frame_buffer = video_mem;

  unsigned int index = (mode_info.XResolution * y + x) * ((mode_info.BitsPerPixel + 7) / 8);

  if (memcpy(&frame_buffer[index], &color, ((mode_info.BitsPerPixel + 7) / 8)) == NULL)
    return 1;

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (int i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0)
      return 1;
  }

  return 0;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (int i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y , color) != 0)
      return 1;
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for (int i = y; i < height + y; i++) {
    if (vg_draw_hline(x, i, width, color) != 0)
      return 1;
  }

  return 0;
}

int get_color(uint32_t color, uint32_t *new_color) {
  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
  }
  else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }
  return 0;
}

// draw pattern

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << mode_info.BitsPerPixel);
}

// combines que r, g, b components according to the distribution

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
}

// obter os valores para R G e B consoante a primeira cor em direct_mode

uint32_t (Red)(unsigned col, uint8_t step, uint32_t first) {
  return (R(first) + col * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (Green)(unsigned row, uint8_t step, uint32_t first) {
  return (G(first) + row * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (Blue)(unsigned col, unsigned row, uint8_t step, uint32_t first) {
  return (B(first) + (row + col) * step) % (1 << mode_info.BlueMaskSize);
}

// obtencao dos componentes R, G e B respetivamente



//     ((1 << 5 - 1 = 100000 - 1 = 011111) & (cor >> Position))
  // RedMaskSie = 5 & RedFieldPosition = 11 & color 16 bits (mode 0x11A 5:6:5) 0b0000 0|000 000|0 0000
  //                                                                               R   |   G   |   B
  // lhs - mask       -> ((1<<5) - 1) = (100000 - 1) = 011111

  // rhs - red field  -> (color >> 11) = red block of color

  // lhs (mask) & rhs (red field) = red component

uint32_t (R)(uint32_t first){
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}

