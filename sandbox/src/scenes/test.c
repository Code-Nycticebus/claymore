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
  (void)test;
}

static void frame_update(CmScene *scene) {
  Test *test = cm_scene_data(scene);
  (void)test;
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
