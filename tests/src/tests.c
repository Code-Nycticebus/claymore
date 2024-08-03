#include "tests/tests.h"
#include "claymore/entrypoint.h"
#include "utils/test.h"

static CmSceneInit test_suite[] = {
    first,
    first,
};

typedef struct {
  u32 idx;
} TestSuite;

static void event(CmScene *scene, CmEvent *event) {
  cm_event_quit(event, {
    if (quit->code == TESTS_OK) {
      cm_scene_delete(cm_scene_child(scene, 0));
      event->handled = true;
    } else if (quit->code == TESTS_ERR) {
      cebus_log_error("TESTS FAILED");
      CmScene *test = cm_scene_child(scene, 0);
      Str name = cm_scene_name(cm_scene_child(test, 0));
      cebus_log_error("FAILED: \"" STR_FMT "\"", STR_ARG(name));
    }
  });
}

static void init(CmScene *scene) { (void)scene; }

static void frame_update(CmScene *scene) {
  TestSuite *suite = cm_scene_data(scene);
  if (suite->idx < ARRAY_LEN(test_suite)) {
    if (cm_scene_children(scene)->len == 0) {
      CmScene *test = test_init(scene, test_suite[suite->idx++]);
      Str name = cm_scene_name(cm_scene_child(test, 0));
      cebus_log_info("TESTING: " STR_FMT, STR_ARG(name));
    }
  } else {
    cebus_log_info("TESTS SUCCESSFULL");
    cm_app_quit(0);
  }
}

static CmSceneInterface *tests(void) {
  static CmSceneInterface interface = {
      .init = init,
      .event = event,
      .frame_update = frame_update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 320, .height = 280, .title = "TESTS"},
      .root = tests,
  };
  return &config;
}
