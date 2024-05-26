#include "claymore/entrypoint.h"

static void init(CmScene *scene) { (void)scene; }
static void frame_update(CmScene *scene, double dt) { (void)scene, (void)dt; }

static CmSceneInterface *basic(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "basic"},
      .main = basic,
  };
  return &config;
}
