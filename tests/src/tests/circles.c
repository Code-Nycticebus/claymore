#include "tests.h"

typedef struct {
  CmCamera2D camera;
} Circles;

static void init(CmScene *scene) {
  Circles *first = cm_scene_data(scene);
  cm_camera2D_screen(&first->camera);
}

static void frame_update(CmScene *scene) {
  Circles *first = cm_scene_data(scene);

  cm_2D_begin(&first->camera);
  cm_2D_circle((vec2){40, 60}, 10, (vec4){0.3f, 0.7f, 0.2f, 1});
  cm_2D_circle((vec2){67, 23}, 15, (vec4){0.84f, 0.25f, 0.245f, 1});
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
