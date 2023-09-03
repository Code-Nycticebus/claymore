#include "claymore.h"
#include "claymore/core/entrypoint.h"

#define WINDOW_WIDTH 620
#define WINDOW_HEIGHT 420

#include "cube/cube.h"
#include "ortho/ortho.h"

static void app_key_event(CmKeyEvent *event, void *data) {
  (void)data;
  if (event->action == CM_KEY_PRESS) {
    if (event->code == CM_KEY_1) {
      cm_scene_change(0);
    }
    if (event->code == CM_KEY_2) {
      cm_scene_change(1);
    }
  }
}

static bool app_init(CmApp *app) {
  (void)app;
  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)app_key_event,
                        NULL);
  return true;
}

ClaymoreConfig claymore_config(void) {
  return (const ClaymoreConfig){
      .window =
          {
              .width = WINDOW_WIDTH,
              .height = WINDOW_HEIGHT,
              .title = "Sandbox",
          },
      .app =
          {
              .init = app_init,
              .free = NULL,
          },
      .scenes =
          {
              scene_ortho,
              scene_cube,
              NULL,
          },
  };
}
