#include "claymore.h"

typedef struct {
  float timer;
  CmFont *font;
  mat4 projection;
} Fps;

static void init(CmScene *scene) {
  Fps *fps = cm_scene_alloc_data(scene, sizeof(Fps));
  fps->timer = 0;
  fps->font = cm_font_init(&scene->gpu, STR("sandbox/res/fonts/Ubuntu.ttf"),
                           24.f, ErrPanic);
  mat4 projection;
  glm_ortho(0, 720, 420, 0, -1.F, 100.F, projection);
  glm_mat4_mul(projection, GLM_MAT4_IDENTITY, fps->projection);
}

static void update(CmScene *scene, double deltatime) {
  Fps *fps = scene->data;
  char buffer[20] = {0};
  usize len = snprintf(buffer, 20, "FPS: % 5.0f", 1 / deltatime);
  cm_font_draw(fps->font, fps->projection, (vec3){10, 24},
               str_from_parts(len, buffer));
  fps->timer = 0;
}

CmSceneInterface *fps_scene_init(void) {
  static CmSceneInterface sandbox = {
      .init = init,
      .update = update,
  };
  return &sandbox;
}
