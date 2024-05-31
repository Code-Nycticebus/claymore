#include "renderer2D.h"

// Hacky but only here to catch errors
const CmCamera2D *current_camera;

void cm_2D_begin(CmCamera2D *camera) {
  if (current_camera != NULL && current_camera != camera) {
    cebus_log_warning(
        "'cm_2D_begin()' was called twice before 'cm_2D_end()' was called!");
  }
  current_camera = camera;
  cm_camera_update(camera);
  cm_quad_internal_begin(camera);
  cm_circle_internal_begin(camera);
  cm_sprite_internal_begin(camera);
  cm_font_internal_begin(camera);
}

void cm_2D_end(void) {
  current_camera = NULL;
  cm_quad_internal_end();
  cm_circle_internal_end();
  cm_sprite_internal_end();
  cm_font_internal_end();
}

bool cm_2D_internal_init(void) {
  usize size = 0;

  size += cm_quad_internal_init();
  size += cm_circle_internal_init();
  size += cm_sprite_internal_init();
  size += cm_font_internal_init();

#if defined(CLAYMORE_DEBUG)
  const usize bytes = size / 1000;
  cebus_log_info("Renderer2D: %" USIZE_FMT " kb", bytes);
#endif

  return true;
}

void cm_2D_internal_free(void) {
  cm_quad_internal_free();
  cm_circle_internal_free();
  cm_sprite_internal_free();
  cm_font_internal_free();
}
