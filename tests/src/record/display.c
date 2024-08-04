#include "display.h"
#include "writer.h"

typedef struct {
  u32 idx;
  u32 test_count;
  const CmSceneInit *tests;
  u32 w, h;
} TestDisplay;

static void event(CmScene *scene, CmEvent *event) {
  TestDisplay *display = cm_scene_data(scene);
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Escape) {
        cm_app_quit(0);
        break;
      }
      if (key->code == RGFW_Right) {
        cm_scene_delete(cm_scene_child(scene, 0));
        display->idx = display->idx < display->test_count - 1 ? display->idx + 1 : 0;
      }
      if (key->code == RGFW_Left) {
        cm_scene_delete(cm_scene_child(scene, 0));
        display->idx = display->idx == 0 ? display->test_count - 1 : display->idx - 1;
      }
      if (key->code == RGFW_Return) {
        cebus_log_info("SAVE");
        writer_push(scene, display->w, display->h, display->tests[display->idx]);
      }
    }
  });
}

static void init(CmScene *scene) {
  TestDisplay *display = cm_scene_data(scene);
  display->idx = 0;
}

static void frame_update(CmScene *scene) {
  TestDisplay *display = cm_scene_data(scene);
  if (cm_scene_children(scene)->len == 0) {
    cm_scene_push(scene, display->tests[display->idx]);
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

CmScene *display(CmScene *parent, u32 width, u32 height, u32 test_count, const CmSceneInit *tests) {
  CmScene *scene = cm_scene_push(parent, interface);
  TestDisplay *display = cm_scene_data(scene);

  cebus_assert(test_count, "At least one test needs to happen");
  display->test_count = test_count;
  display->tests = tests;

  display->w = width;
  display->h = height;

  return scene;
}