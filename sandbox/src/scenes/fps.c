#include "claymore.h"

#define BUFFER_MAX 40

typedef struct {
  CmFont *font;
  mat4 projection;
} Fps;

const float font_heigth = 24.f;
const vec2 offset = {10, 0};

static void init(CmScene *scene) {
  Fps *fps = cm_scene_alloc_data(scene, sizeof(Fps));
  fps->font = cm_font_init(&scene->gpu, STR("res/fonts/Ubuntu.ttf"),
                           font_heigth, ErrPanic);

  vec2 window_size;
  cm_window_get_size(window_size);

  mat4 projection;
  glm_ortho(0, window_size[0], window_size[1], 0, -1.F, 100.F, projection);
  glm_mat4_mul(projection, GLM_MAT4_IDENTITY, fps->projection);
}

static void update(CmScene *scene, double deltatime) {
  Fps *fps = scene->data;
  char buffer[BUFFER_MAX] = {0};
  const float ms = deltatime * 1000;
  usize len = snprintf(buffer, BUFFER_MAX, "FRAME: % 3.2f ms\nFPS: %.0f", ms,
                       1 / deltatime);
  cm_font_draw(fps->font, fps->projection, offset, str_from_parts(len, buffer));
}

CmSceneInterface *fps_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
  };
  return &sandbox;
}
