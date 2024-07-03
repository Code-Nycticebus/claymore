#include "claymore.h"

typedef struct {
  CmCamera2D camera;
} Test;

static void event(CmScene *scene, CmEvent *event) {
  Test *test = cm_scene_data(scene);
  (void)test, (void)event;
}

static void init(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  cm_camera2D_screen(&test->camera);
}

static void frame_update(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  (void)test;

  cm_2D_begin(&test->camera);

  cm_2D_line((vec2){0, 0}, (vec2){100, 50});
  cm_2D_line((vec2){100, 150}, (vec2){200, 330});

  cm_2D_end();
}

CmSceneInterface *test(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Test),
      .init = init,
      .frame_update = frame_update,
      .event = event,
  };
  return &interface;
}
