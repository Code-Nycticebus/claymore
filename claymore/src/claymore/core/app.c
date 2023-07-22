#include "app.h"

#include "claymore/debug/debug.h"

#include "claymore/core/window.h"
#include "claymore/events/event.h"
#include "claymore/events/event_types.h"
#include "claymore/logger/logger.h"
#include "claymore/renderer/context.h"
#include <stdio.h>

static void _cm_app_key_event(CmApp *app, const CmKeyEvent *key) {
  (void)app;
  if (key->action == CM_KEY_PRESS || key->action == CM_KEY_REPEAT) {
    CM_TRACE("Pressed %d\n", key->code);
  }
}
static void _cm_app_mouse_event(CmApp *app, const CmMouseEvent *mouse) {
  (void)mouse, (void)app;
}

static void _cm_app_window_close(CmApp *app, const CmWindowEvent *window) {
  (void)app;
  cm_window_close(window->window);
}

bool cm_app_init(CmApp *app, const ClaymoreConfig *config) {
  cm_logger_init();

  CM_FATAL("%s", "FUCK");
  CM_INFO("%s\n", cm_debug_enabled ? "Debug" : "Release");

  cm_event_init(app);
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)_cm_app_key_event);

  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)_cm_app_mouse_event);

  cm_event_set_callback(CM_EVENT_WINDOW_CLOSE,
                        (cm_event_callback)_cm_app_window_close);

  app->window = cm_window_init(config->window.width, config->window.height,
                               config->window.title);
  if (app->window == NULL) {
    return false;
  }
  return true;
}

void cm_app_shutdown(CmApp *app) { cm_window_close(app->window); }
