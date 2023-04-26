// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/video_gr.h>

extern vbe_mode_info_t mode_info;
void* video_mem;

void *(vg_init)(uint16_t mode) {

    if (vbe_get_mode_info(mode, &mode_info) != 0) {
        printf ("Failed at getting mode info\n.");
        return NULL;
    }

    /* Lowest memory address in range */
    unsigned int vram_base = mode_info.PhysBasePtr;
    // rounding bytes by excess ((bit+7)/8)
    unsigned int vram_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel+7)/8);

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
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
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
    r86.bx = BIT(14) | mode;

    fail = sys_int86(&r86);
    if (fail) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }

    return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    
    char *position = video_mem;
    position += (mode_info.XResolution * y + x) * ((mode_info.BitsPerPixel + 7) / 8); 
    memcpy(position, &color, ((mode_info.BitsPerPixel + 7) / 8));

    for(int i = 0 ; i < len ; i++ ) {
        position += ((mode_info.BitsPerPixel + 7) / 8);
        memcpy(position, &color, ((mode_info.BitsPerPixel + 7) / 8));
    }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    
    for (int i = y; i < height + y; i++) {
        if (vg_draw_hline(x, i, width, color) != 0)
            return 1;
    }

    return 0;
}
