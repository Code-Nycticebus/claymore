#include "claymore/entrypoint.h"

#include "cases/cases.h"
#include "record/record.h"

static void init(CmScene *scene) {
  display(scene, WIDHT, HEIGHT, ARRAY_LEN(test_cases), test_cases);
}

static CmSceneInterface *root(void) {
  static CmSceneInterface interface = {
      .init = init,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = WIDHT * 4, .height = HEIGHT * 4, .title = "DISPLAY"},
      .root = root,
  };
  return &config;
}
