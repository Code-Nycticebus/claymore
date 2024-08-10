#ifndef __CLAYMORE_TEXTURE2D_H__
#define __CLAYMORE_TEXTURE2D_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/renderer/gpu.h"

typedef struct {
  u32 w, h;
  u32 bpp;

  struct {
    GLenum mag;
    GLenum min;
  } filter;

  struct {
    GLenum s;
    GLenum t;
  } wrap;
} CmTextureFormat;

typedef struct {
  u32 id;
  u32 width;
  u32 height;
  u32 bpp;
} CmTexture;

CmTexture cm_texture_from_file(CmGpu *gpu, Str filename, Error *error);
CmTexture cm_texture_from_bytes(CmGpu *gpu, Bytes bytes, CmTextureFormat format);

Bytes cm_texture_read_bytes(CmTexture *texture, Arena *arena);

void cm_texture_bind(CmTexture *texture, uint32_t slot);
void cm_texture_unbind(uint32_t slot);

#endif /* !__CLAYMORE_TEXTURE2D_H__ */
