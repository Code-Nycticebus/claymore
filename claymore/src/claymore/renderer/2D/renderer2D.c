#include "renderer2D.h"

static CmRenderer2D *renderer;

void cm_2D_begin(CmCamera2D *camera) {
  if (renderer->camera != NULL && renderer->camera != camera) {
    cebus_log_error(
        FILE_LOCATION_FMT
        ": 'cm_2D_begin()' was called twice before 'cm_2D_end()' was called",
        FILE_LOCATION_ARG_CURRENT);
    DEBUGBREAK();
  }
  renderer->camera = camera;
  cm_camera_update(camera);
  cm_quad_internal_begin(camera);
  cm_circle_internal_begin(camera);
  cm_sprite_internal_begin(camera);
  cm_font_internal_begin(camera);
  cm_line_internal_begin(camera);
}

void cm_2D_end(void) {
  if (renderer->camera == NULL) {
    cebus_log_error(FILE_LOCATION_FMT ": 'cm_2D_begin()' was never called",
                    FILE_LOCATION_ARG_CURRENT);
    DEBUGBREAK();
  }
  renderer->camera = NULL;
  cm_quad_internal_end();
  cm_circle_internal_end();
  cm_sprite_internal_end();
  cm_font_internal_end();
  cm_line_internal_end();
}

void cm_2D_internal_use(CmRenderer2D *r) {
  renderer = r;
  cm_circle_internal_use(renderer->circle);
  cm_quad_internal_use(renderer->quad);
  cm_sprite_internal_use(renderer->sprite);
  cm_font_internal_use(renderer->font);
  cm_line_internal_use(renderer->font);
}

CmRenderer2D *cm_2D_internal_init(void) {
  renderer = calloc(1, sizeof(CmRenderer2D));

  renderer->quad = cm_quad_internal_init();
  renderer->circle = cm_circle_internal_init();
  renderer->sprite = cm_sprite_internal_init();
  renderer->font = cm_font_internal_init();
  renderer->line = cm_line_internal_init();

  return renderer;
}

void cm_2D_internal_free(void) {
  cm_quad_internal_free();
  cm_circle_internal_free();
  cm_sprite_internal_free();
  cm_font_internal_free();
  cm_line_internal_free();

  free(renderer);
}
