#include "fps.h"

typedef struct {
  CmFont *font;
  CmCamera2D camera;
  vec2 position;
  double time;
  double deltas;
  u32 count;
  Str label;
} Fps;

static void init(CmScene *scene) {
  Fps *fps = cm_scene_data(scene);
  cm_camera2D_screen(&fps->camera);
}

static void frame_update(CmScene *scene) {
  Fps *fps = cm_scene_data(scene);

  fps->time += cm_app_deltatime();
  fps->count++;
  const double interval = .1f;
  if (interval <= fps->time) {
    arena_reset(&scene->arena);

    const float a = fps->time / fps->count;
    const float ms = a * 1000;
    fps->label = str_format(&scene->arena, "FPS: %.0f, % 3.2f ms", 1 / a, ms);

    fps->time = 0;
    fps->count = 0;
  }

  cm_2D_begin(&fps->camera);
  cm_2D_text(fps->font, fps->position, fps->label);
  cm_2D_end();
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Fps),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

CmScene *fps(CmScene *parent, const vec2 position, Str font, float heigth) {
  CmScene *scene = cm_scene_push(parent, interface);
  Fps *fps = cm_scene_data(scene);

  fps->position[0] = position[0];
  fps->position[1] = position[1];

  fps->font = cm_font_from_file(&scene->gpu, font, heigth, ErrPanic);

  return scene;
}
