#include "fps.h"

#define SIZE 40

typedef struct {
  CmFont *font;
  CmCamera2D camera;
  vec2 position;
  double time;
  double deltas;
  u32 count;
  char buffer[SIZE];
  Str label;
} Fps;

static void init(CmScene *scene) {
  Fps *fps = cm_scene_set_data(scene, sizeof(Fps));
  cm_camera2D_screen(&fps->camera);
}

static void frame_update(CmScene *scene) {
  Fps *fps = scene->data;

  fps->time += cm_app_deltatime();
  fps->count++;
  const double interval = .1f;
  if (interval <= fps->time) {
    const float a = fps->time / fps->count;
    const float ms = a * 1000;
    usize len = snprintf(fps->buffer, SIZE, "FPS: %.0f, % 3.2f ms", 1 / a, ms);
    fps->label = str_from_parts(len, fps->buffer);

    fps->time = 0;
    fps->count = 0;
  }

  cm_2D_begin(&fps->camera);
  cm_font(fps->font, fps->position, fps->label);
  cm_2D_end();
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

CmScene *fps(CmScene *parent, const vec2 position, Str font, float heigth) {
  CmScene *scene = cm_scene_push(parent, interface);
  Fps *fps = scene->data;

  fps->position[0] = position[0];
  fps->position[1] = position[1];

  fps->font = cm_font_from_file(&scene->gpu, font, heigth, ErrPanic);

  return scene;
}
