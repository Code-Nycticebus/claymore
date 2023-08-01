#include "app.h"

#include "cm.h"

#include <stdio.h>

static void _cm_app_key_event(CmApp *app, CmKeyEvent *key) {
  (void)app;
  if (key->action == CM_KEY_PRESS || key->action == CM_KEY_REPEAT) {
    CM_TRACE("Pressed %d\n", key->code);
  }
}

static void _cm_app_window_close(CmApp *app, CmWindowEvent *window) {
  (void)window;
  app->run = false;
}

bool cm_app_init(CmApp *app, const ClaymoreConfig *config) {
  cm_logger_init();

  cm_event_init(app);
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)_cm_app_key_event);

  cm_event_set_callback(CM_EVENT_WINDOW_CLOSE,
                        (cm_event_callback)_cm_app_window_close);

  app->window = cm_window_init(config->window.width, config->window.height,
                               config->window.title);
  if (app->window == NULL) {
    return false;
  }

  CM_INFO("%s\n", cm_debug_enabled ? "Claymore Debug" : "Claymore Release");

  app->run = true;
  return true;
}

void cm_app_shutdown(CmApp *app) { cm_window_close(app->window); }
