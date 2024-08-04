#include "tests/tests.h"
#include "claymore/entrypoint.h"

#include "record/display.h"
#include "suite/suite.h"

static void init(CmScene *scene) {
  (void)scene;
#if 0
  suite(scene, ARRAY_LEN(test_suite), test_suite);
#else
  display(scene, ARRAY_LEN(test_suite), test_suite);
#endif
}

static CmSceneInterface *tests(void) {
  static CmSceneInterface interface = {
      .init = init,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 100, .height = 100, .title = "TESTS"},
      .root = tests,
  };
  return &config;
}
