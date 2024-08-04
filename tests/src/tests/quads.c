#include "tests.h"

typedef struct {
  CmCamera2D camera;
} Quads;

static void init(CmScene *scene) {
  Quads *first = cm_scene_data(scene);
  cm_camera2D_ortho(&first->camera, (vec2){0}, (f32)HEIGHT / (f32)WIDHT, 5);
}

static void frame_update(CmScene *scene) {
  Quads *first = cm_scene_data(scene);

  cm_2D_begin(&first->camera);

  cm_2D_quad((vec2){-3, -2}, (vec2){2, 2}, (vec4){0.7f, 0.2f, 0.2f, 1});
  cm_2D_quad_rotated((vec2){2, 1}, (vec2){2, 2}, glm_rad(45), (vec4){0.25f, 0.25f, 0.8f, 1});

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
