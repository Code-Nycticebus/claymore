#include "tests/tests.h"
#include "claymore/entrypoint.h"
#include "utils/test.h"

static void init(CmScene *scene) { test_init(scene, first); }

static CmSceneInterface *tests(void) {
  static CmSceneInterface interface = {
      .init = init,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "TESTS"},
      .root = tests,
  };
  return &config;
}
