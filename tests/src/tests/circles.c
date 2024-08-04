#include "tests.h"

typedef struct {
  CmCamera2D camera;
} Circles;

static void init(CmScene *scene) {
  Circles *first = cm_scene_data(scene);
  cm_camera2D_ortho(&first->camera, (vec2){0}, (f32)HEIGHT / (f32)WIDHT, 10);
}

static void frame_update(CmScene *scene) {
  Circles *first = cm_scene_data(scene);

  cm_2D_begin(&first->camera);
  cm_2D_circle((vec2){-8, 2}, 1, (vec4){0.3f, 0.7f, 0.2f, 1});
  cm_2D_circle((vec2){2, 1}, 1, (vec4){0.84f, 0.25f, 0.245f, 1});
  cm_2D_circle((vec2){4, -5}, 1, (vec4){0.84f, 0.731f, 0.245f, 1});
  cm_2D_end();
}

CmSceneInterface *circles(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Circles),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
