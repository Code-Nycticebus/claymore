#include "tests.h"

typedef struct {
  CmCamera2D camera;
} Quads;

static void init(CmScene *scene) {
  Quads *first = cm_scene_data(scene);
  cm_camera2D_screen(&first->camera);
}

static void frame_update(CmScene *scene) {
  Quads *first = cm_scene_data(scene);

  cm_2D_begin(&first->camera);

  cm_2D_quad((vec2){20, 20}, (vec2){20, 20}, (vec4){0.7f, 0.2f, 0.2f, 1});
  cm_2D_quad_rotated((vec2){50, 40}, (vec2){20, 20}, glm_rad(45), (vec4){0.25f, 0.25f, 0.8f, 1});

  cm_2D_end();
}

CmSceneInterface *quads(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Quads),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
