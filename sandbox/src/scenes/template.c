#include "claymore.h"

static void init(CmScene *scene) { (void)scene; }
static void frame_update(CmScene *scene) { (void)scene; }

CmSceneInterface *template(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
