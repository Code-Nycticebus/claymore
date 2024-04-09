#include "claymore/entrypoint.h"

void init(CmScene *scene) { (void)scene; }

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface benchmark = {
      .init = init,
  };
  return &benchmark;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "basic"},
      .main = scene_init,
  };
  return &config;
}
