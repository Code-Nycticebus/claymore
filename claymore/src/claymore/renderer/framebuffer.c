#include "framebuffer.h"

CmFramebuffer cm_framebuffer_create(CmGpu *gpu, usize width, usize heigth) {
  CmFramebuffer fb = {.gpu = gpu, .size = {width, heigth}};
  fb.fbo = cm_gpu_fbo(gpu);
  cm_gpu_fbo_bind(0);
  return fb;
}

CmGpuID cm_framebuffer_attach_texture_color(CmFramebuffer *fb) {
  cm_gpu_fbo_bind(fb->fbo);

  CmGpuID id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->size[0], fb->size[1], 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0 + fb->attachment_count++,
                         GL_TEXTURE_2D, id, 0);
  cm_gpu_fbo_bind(0);

  fb->mask |= GL_COLOR_BUFFER_BIT;
  return id;
}

void cm_framebuffer_begin(CmFramebuffer *fb) {
  cebus_assert_debug(fb->mask, "framebuffer mask was never set");
  cm_gpu_fbo_bind(fb->fbo);
  glClear(fb->mask);
}

void cm_framebuffer_end(void) { cm_gpu_fbo_bind(0); }
