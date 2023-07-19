#include "app.h"

#include "claymore/core/window.h"
#include "claymore/debug/debug.h"

#include "claymore/events/event.h"
#include "claymore/renderer/context.h"
#include <stdio.h>

static void _cm_app_key_event(const CmKeyEvent *key) {
  if (key->action == CM_KEY_PRESS || key->action == CM_KEY_REPEAT) {
    printf("Pressed %d\n", key->code);
  }
}
static void _cm_app_mouse_click_event(const CmMouseInfo *mouse) {
  printf("mouse clicked on: (%f, %f)\n", mouse->x, mouse->y);
}

static void _cm_app_mouse_move_event(const CmMouseInfo *mouse) {
  printf("mouse x: %f y: %f\n", mouse->x, mouse->y);
}
ClaymoreApp claymore_app_init(const ClaymoreConfig *config) {
  ClaymoreApp app;

  cm_logger_init(config->log.out, config->log.err);

  cm_log_dbg("%s\n", cm_debug_enabled ? "Debug" : "Release");

  app.window.widht = config->window.width;
  app.window.height = config->window.height;
  app.window.ctx = cm_window_init(config->window.width, config->window.height,
                                  config->window.title);

  if (app.window.ctx == NULL) {
    return (ClaymoreApp){0};
  }

  if (!cm_context_init(app.window.ctx)) {
    return (ClaymoreApp){0};
  }

  cm_event_init();
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)_cm_app_key_event);

  cm_event_set_callback(CM_EVENT_MOUSE_CLICK,
                        (cm_event_callback)_cm_app_mouse_click_event);
  cm_event_set_callback(CM_EVENT_MOUSE_MOVE,
                        (cm_event_callback)_cm_app_mouse_move_event);
  return app;
}
