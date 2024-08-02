#include "tests.h"

static void init(CmScene *scene) {
  (void)scene;
  cebus_log("YES");
}
static void frame_update(CmScene *scene) { (void)scene; }

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      .name = STR_STATIC("First"),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

CmScene *first(CmScene *parent) { return cm_scene_push(parent, interface); }
