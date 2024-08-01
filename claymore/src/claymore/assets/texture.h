#ifndef __CLAYMORE_TEXTURE2D_H__
#define __CLAYMORE_TEXTURE2D_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/renderer/gpu.h"

typedef enum {
  CM_TEXTURE_R,
  CM_TEXTURE_RGBA,
  CM_TEXTURE_RGB,
} CmTextureFormat;

typedef struct {
  u32 id;
  usize width;
  usize height;
} CmTexture;

CmTexture cm_texture_from_file(CmGpu *gpu, Str filename, Error *error);
CmTexture cm_texture_from_memory(CmGpu *gpu, usize w, usize h, const u8 *data,
                                 CmTextureFormat format);

void cm_texture_bind(CmTexture *texture, uint32_t slot);
void cm_texture_unbind(uint32_t slot);

#endif /* !__CLAYMORE_TEXTURE2D_H__ */
