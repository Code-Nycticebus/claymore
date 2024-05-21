#include "fps.h"

#define BUFFER_MAX 40

typedef struct {
  CmFont *font;
  CmCamera2D camera;
} Fps;

const float font_heigth = 24.f;
const vec2 offset = {10, 0};

static void init(CmScene *scene) {
  Fps *fps = cm_scene_alloc_data(scene, sizeof(Fps));
  fps->font = cm_font_init(&scene->gpu, STR("res/fonts/Ubuntu.ttf"),
                           font_heigth, ErrPanic);

  cm_camera2d_screen(&fps->camera);
}

static void update(CmScene *scene, double deltatime) {
  Fps *fps = scene->data;
  char buffer[BUFFER_MAX] = {0};
  const float ms = deltatime * 1000;
  usize len = snprintf(buffer, BUFFER_MAX, "FRAME: % 3.2f ms\nFPS: %.0f", ms,
                       1 / deltatime);
  cm_renderer2d_begin(&fps->camera);
  cm_font(fps->font, offset, str_from_parts(len, buffer));
  cm_renderer2d_end();
}

CmSceneInterface *fps(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
  };
  return &sandbox;
}
