#include "claymore/entrypoint.h"

void init(CmScene *scene) { (void)scene; }

static CmSceneInterface *basic(void) {
  static CmSceneInterface interface = {
      .init = init,
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
