#include "suite.h"

typedef struct {
  Error error;
  u32 idx;
  u32 test_count;
  const CmSceneInit *tests;
  u32 w, h;
} TestDisplay;

static void init(CmScene *scene) {
  TestDisplay *suite = cm_scene_data(scene);
  suite->idx = 0;
  suite->error = ErrNew;
}

static void frame_update(CmScene *scene) {
  TestDisplay *suite = cm_scene_data(scene);

  if (suite->idx < suite->test_count) {
    if (cm_scene_children(scene)->len == 0) {
      test_init(scene, suite->w, suite->h, &suite->error, suite->tests[suite->idx++]);
    }
  } else {
    cebus_log_info("ALL TESTS SUCCESSFULL");
    cm_app_quit(0);
  }

  error_context(&suite->error, {
    if (error_code(TestsExitCodes) == TESTS_FAILED) {
      Str msg = error_msg();
      cebus_log_error(STR_FMT, STR_ARG(msg));
      cm_app_set_root(suite->tests[suite->idx - 1]);
      error_except();
    }
  });
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(TestDisplay),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

CmScene *suite(CmScene *parent, u32 width, u32 height, u32 test_count, const CmSceneInit *tests) {
  CmScene *scene = cm_scene_push(parent, interface);
  TestDisplay *suite = cm_scene_data(scene);

  suite->test_count = test_count;
  suite->tests = tests;

  suite->w = width;
  suite->h = height;

  return scene;
}
