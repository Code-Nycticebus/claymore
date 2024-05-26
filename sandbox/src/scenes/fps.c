#include "fps.h"

#define SIZE 40

typedef struct {
  CmFont *font;
  CmCamera2D camera;
} Fps;

static const Str font = STR("assets/fonts/Ubuntu.ttf");
const float font_heigth = 24.f;
const vec2 offset = {10, 0};

static void init(CmScene *scene) {
  Fps *fps = cm_scene_set_data(scene, sizeof(Fps));
  fps->font = cm_font_init(&scene->gpu, font, font_heigth, ErrPanic);

  cm_camera2D_screen(&fps->camera);
}

static void frame_update(CmScene *scene, double dt) {
  Fps *fps = scene->data;

  char buffer[SIZE] = {0};
  const float ms = dt * 1000;
  usize len = snprintf(buffer, SIZE, "FRAME: % 3.2f ms\nFPS: %.0f", ms, 1 / dt);
  cm_2D_begin(&fps->camera);
  cm_font(fps->font, offset, str_from_parts(len, buffer));
  cm_2D_end();
}

CmSceneInterface *fps(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .frame_update = frame_update,
  };
  return &sandbox;
}
