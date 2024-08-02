#ifndef __CLAYMORE_FRAMEBUFFER_H__
#define __CLAYMORE_FRAMEBUFFER_H__

#include "claymore/assets/texture.h"
#include "claymore/renderer/gpu.h"

typedef struct {
  CmGpu *gpu;
  CmGpuID fbo;
  usize size[2];
  GLbitfield mask;
  usize attachment_count;
} CmFramebuffer;

CmFramebuffer cm_framebuffer_create(CmGpu *gpu, usize width, usize heigth);
CmTexture cm_framebuffer_attach_texture_color(CmFramebuffer *fb);

void cm_framebuffer_begin(CmFramebuffer *fb);
void cm_framebuffer_end(void);

#endif /* __CLAYMORE_FRAMEBUFFER_H__ */
