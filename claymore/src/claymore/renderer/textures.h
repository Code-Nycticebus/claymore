#ifndef __CM_TEXTURES_H__
#define __CM_TEXTURES_H__

#include "cm.h"

typedef struct {
  uint32_t id;
  int32_t width;
  int32_t height;
  int32_t bpp;
} CmTexture2D;

CmTexture2D cm_texture2d_create(const char *filename);
void cm_texture_delete(CmTexture2D *texture);
void cm_texture_bind(CmTexture2D *texture, uint32_t slot);
void cm_texture_unbind(uint32_t slot);

#endif /* !__CM_TEXTURES_H__ */
