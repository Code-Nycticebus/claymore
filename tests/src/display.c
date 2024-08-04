#include "claymore/entrypoint.h"
#include "tests/tests.h"

#include "record/display.h"

static void init(CmScene *scene) {
  display(scene, WIDHT, HEIGHT, ARRAY_LEN(test_suite), test_suite);
}

static CmSceneInterface *root(void) {
  static CmSceneInterface interface = {
      .init = init,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = WIDHT, .height = HEIGHT, .title = "DISPLAY"},
      .root = root,
  };
  return &config;
}
