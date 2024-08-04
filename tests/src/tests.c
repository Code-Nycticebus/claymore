#include "claymore/entrypoint.h"

#include "cases/cases.h"
#include "suite/suite.h"

static void init(CmScene *scene) { suite(scene, WIDHT, HEIGHT, ARRAY_LEN(test_cases), test_cases); }

static CmSceneInterface *root(void) {
  static CmSceneInterface interface = {
      .init = init,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = WIDHT, .height = HEIGHT, .title = "TESTS"},
      .root = root,
  };
  return &config;
}
