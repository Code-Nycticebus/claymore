#include "claymore.h"
#include "claymore/core/entrypoint.h"

#include "gui/gui.h"
#include "instancing/instancing.h"
#include "light/light.h"
#include "menu/menu.h"
#include "models/models.h"
#include "noise/noise.h"
#include "ortho/ortho.h"
#include "particles/particle.h"
#include "texture/texture.h"

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720

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
              .update = NULL,
              .free = NULL,
          },
      .scenes =
          {
              scene_menu,
              scene_ortho,
              scene_texture,
              scene_particle,
              scene_instancing,
              gui_scene,
              scene_models,
              scene_light,
              scene_noise,
          },
  };
}
