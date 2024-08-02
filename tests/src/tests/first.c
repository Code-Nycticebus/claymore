#include "tests.h"

#include "stb_image_write.h"

static void init(CmScene *scene) {
  (void)scene;
  cebus_log("YES");

  stbi_write_png("test.png", 2, 2, 3,
                 (u8[]){
                     255, 255, 0, //
                     255, 0, 255, //
                     255, 0, 255, //
                     255, 255, 0, //
                 },
                 2 * 3);
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
