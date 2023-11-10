#ifndef __CM_TEXTURES_H__
#define __CM_TEXTURES_H__

#include "cm.h"

typedef enum {
  CM_TEX_R,
  CM_TEX_RGBA,
} CmTextureFormat;

typedef struct {
  uint32_t id;
  uint32_t width;
  uint32_t height;
  uint32_t bpp;
  CmTextureFormat format;
} CmTexture;

CmTexture cm_texture_create(const char *filename);
CmTexture cm_texture_create_from_memory(uint32_t width, uint32_t height,
                                        const void *data,
                                        CmTextureFormat format);
void cm_texture_update(CmTexture *texture, const void *data);

void cm_texture_delete(CmTexture *texture);
void cm_texture_bind(CmTexture *texture, uint32_t slot);
void cm_texture_unbind(uint32_t slot);

#endif /* !__CM_TEXTURES_H__ */
