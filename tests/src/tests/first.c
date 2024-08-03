#include "tests.h"

typedef struct {
  CmCamera2D camera;
} First;

static void init(CmScene *scene) {
  First *first = cm_scene_data(scene);
  cm_camera2D_screen(&first->camera);
}

static void frame_update(CmScene *scene) {
  First *first = cm_scene_data(scene);

  cm_2D_begin(&first->camera);

  cm_2D_quad((vec2){0, 0}, (vec2){100, 100.f}, (vec4){0.7f, 0.2f, 0.2f, 1});
  cm_2D_quad((vec2){310, 90}, (vec2){100, 100.f}, (vec4){0.25f, 0.25f, 0.8f, 1});
  cm_2D_circle((vec2){140, 260}, 50, (vec4){0.3f, 0.7f, 0.2f, 1});

  cm_2D_line((vec2){0, 0}, (vec2){100, 50});
  cm_2D_line((vec2){100, 150}, (vec2){200, 330});

  cm_2D_end();
}

CmSceneInterface *first(void) {
  static CmSceneInterface interface = {
      CM_SCENE(First),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
