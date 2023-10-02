#include "claymore.h"
#include "claymore/core/entrypoint.h"

#define WINDOW_WIDTH 620
#define WINDOW_HEIGHT 420

#include "cube/cube.h"
#include "framebuffer/framebuffer.h"
#include "menu/menu.h"
#include "ortho/ortho.h"

static void app_key_event(CmKeyEvent *event, void *data) {
  (void)data;
  if (event->action == CM_KEY_PRESS) {
    if (event->code == CM_KEY_ESCAPE) {
      cm_scene_change(0);
    }
  }
}

static bool app_init(CmApp *app) {
  (void)app;
  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)app_key_event, NULL);
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
              scene_menu,
              scene_ortho,
              scene_cube,
              scene_framebuffer,
              NULL,
          },
  };
}
