#include "renderer2d.h"

void cm_renderer2d_begin(CmCamera2D *camera) {
  cm_camera_update(camera);
  cm_quad_internal_begin(camera);
  cm_sprite_internal_begin(camera);
  cm_font_internal_begin(camera);
}

void cm_renderer2d_end(void) {
  cm_quad_internal_end();
  cm_sprite_internal_end();
  cm_font_internal_end();
}

bool cm_renderer2d_internal_init(void) {
  usize size = 0;

  size += cm_quad_internal_init();
  size += cm_sprite_internal_init();
  size += cm_font_internal_init();

#if defined(CLAYMORE_DEBUG)
  const usize bytes = size / 1000;
  cebus_log_info("Renderer2D: %" USIZE_FMT " kb", bytes);
#endif

  return true;
}

void cm_renderer2d_internal_free(void) {
  cm_quad_internal_free();
  cm_sprite_internal_free();
  cm_font_internal_free();
}
