#include "renderer2d.h"

bool cm_renderer2d_internal_init(void) {
  usize size = 0;

  size += cm_quad_internal_init();
  size += cm_sprite_internal_init();

#if defined(CLAYMORE_DEBUG)
  const usize bytes = size / 1000;
  clib_log_info("Renderer2D: %" USIZE_FMT " kb", bytes);
#endif

  return true;
}

void cm_renderer2d_internal_free(void) {
  cm_quad_internal_free();
  cm_sprite_internal_free();
}
