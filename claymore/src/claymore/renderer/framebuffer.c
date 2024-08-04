#include "framebuffer.h"

// TODO: very hacky
static CmGpuID current = 0;

CmFramebuffer cm_framebuffer_create(CmGpu *gpu, usize width, usize heigth) {
  CmFramebuffer fb = {.gpu = gpu, .size = {width, heigth}};
  fb.fbo = cm_gpu_fbo(gpu);
  cm_gpu_fbo_bind(0);
  return fb;
}

CmTexture cm_framebuffer_attach_texture_color(CmFramebuffer *fb) {
  cm_gpu_fbo_bind(fb->fbo);

  CmTexture texture = cm_texture_from_bytes(
      fb->gpu, NULL, (CmTextureFormat){.bpp = 4, .w = fb->size[0], .h = fb->size[1]});

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + fb->attachment_count++,
                         GL_TEXTURE_2D, texture.id, 0);
  cm_gpu_fbo_bind(0);

  fb->mask |= GL_COLOR_BUFFER_BIT;
  return texture;
}

void cm_framebuffer_begin(CmFramebuffer *fb) {
  cebus_assert_debug(fb->mask, "framebuffer mask was never set");
  fb->last = current;
  current = fb->fbo;
  cm_gpu_fbo_bind(fb->fbo);
  glClear(fb->mask);
}

void cm_framebuffer_end(CmFramebuffer *fb) {
  current = fb->last;
  cm_gpu_fbo_bind(fb->last);
}
