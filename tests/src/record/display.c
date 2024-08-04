#include "display.h"
#include "writer.h"

typedef struct {
  u32 idx;
  u32 test_count;
  const CmSceneInit *tests;
  FILE *record_file;
} TestDisplay;

static void event(CmScene *scene, CmEvent *event) {
  TestDisplay *suite = cm_scene_data(scene);
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Escape) {
        cm_app_quit(0);
        break;
      }
      if (key->code == RGFW_Right) {
        cm_scene_delete(cm_scene_child(scene, 0));
        suite->idx = suite->idx < suite->test_count - 1 ? suite->idx + 1 : 0;
      }
      if (key->code == RGFW_Left) {
        cm_scene_delete(cm_scene_child(scene, 0));
        suite->idx = suite->idx == 0 ? suite->test_count - 1 : suite->idx - 1;
      }
      if (key->code == RGFW_Return) {
        cebus_log_info("SAVE");
        writer_push(scene, suite->record_file, suite->tests[suite->idx]);
      }
    }
  });
}

static void init(CmScene *scene) {
  TestDisplay *suite = cm_scene_data(scene);
  suite->idx = 0;
}

static void frame_update(CmScene *scene) {
  TestDisplay *suite = cm_scene_data(scene);
  if (cm_scene_children(scene)->len == 0) {
    cm_scene_push(scene, suite->tests[suite->idx]);
  }
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(TestDisplay),
      .init = init,
      .frame_update = frame_update,
      .event = event,
  };
  return &interface;
}

CmScene *display(CmScene *parent, u32 test_count, const CmSceneInit *tests) {
  CmScene *scene = cm_scene_push(parent, interface);
  TestDisplay *suite = cm_scene_data(scene);

  cebus_assert(test_count, "At least one test needs to happen");
  suite->test_count = test_count;
  suite->tests = tests;

  return scene;
}
