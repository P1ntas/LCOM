#include "bitmap.h"

BitMap *create_bitmap(xpm_map_t bitmap){

  BitMap *bmap = (BitMap *) malloc (sizeof(BitMap));
  if(bmap == NULL ) return NULL;

  xpm_image_t img;
  bmap->colors = (uint32_t *) xpm_load(bitmap, XPM_8_8_8_8, &img);
  bmap->height = img.height;
  bmap->width = img.width;

  if( bmap->colors == NULL ) {
    free(bmap);
    return NULL;
  }
  return bmap;
}

void destroy_bitmap(BitMap *bitmap) {
    if (bitmap == NULL) return;
    if (bitmap->colors) free(bitmap->colors);
    free(bitmap);
    bitmap = NULL;
}
