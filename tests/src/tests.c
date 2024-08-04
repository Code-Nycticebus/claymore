#include "tests/tests.h"
#include "claymore/entrypoint.h"

#include "record/display.h"
#include "suite/suite.h"

static void init(CmScene *scene) {
  (void)scene;
#if 0
  suite(scene, WIDHT, HEIGHT, ARRAY_LEN(test_suite), test_suite);
#else
  display(scene, WIDHT, HEIGHT, ARRAY_LEN(test_suite), test_suite);
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
      .window = {.width = WIDHT, .height = HEIGHT, .title = "TESTS"},
      .root = tests,
  };
  return &config;
}
