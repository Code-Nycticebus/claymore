#include "app.h"

#include "claymore/core/camera.h"
#include "claymore/logger/logger.h"
#include "claymore/sound/sound.h"
#include "cm.h"

#include "claymore/renderer/renderer.h"

#include <stdio.h>

static void _cm_app_key_event(CmKeyEvent *key, CmApp *app) {
  (void)app;
  if (key->action == CM_KEY_PRESS || key->action == CM_KEY_REPEAT) {
    cm_log_trace("Pressed %d\n", key->code);
  }
}

static void _cm_app_window_close(CmWindowEvent *window, CmApp *app) {
  (void)window;
  app->run = false;
}

bool cm_app_init(CmApp *app, const ClaymoreConfig *config) {
  cm_logger_init();

  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)_cm_app_key_event,
                        app);

  cm_event_set_callback(CM_EVENT_WINDOW_CLOSE,
                        (cm_event_callback)_cm_app_window_close, app);

  app->window = cm_window_init(config->window.width, config->window.height,
                               config->window.title);
  if (app->window == NULL) {
    return false;
  }

#if defined(__x86_64__) || defined(_M_X64)
  cm_log_info("64-bit\n");
#elif defined(__i386__) || defined(_MIX86)
  cm_log_info("32-bit\n");
#endif
  cm_log_info("%s\n", cm_debug_enabled ? "Claymore Debug" : "Claymore Release");

  if (!cm_sound_init()) {
    return false;
  }

  app->run = true;
  return true;
}

void cm_app_shutdown(CmApp *app) { cm_window_close(app->window); }

void cm_app_run(CmApp *app, const ClaymoreConfig *config) {
  // Layer stack
  struct {
    CmLayerInterface interface;
    CmLayer layer;
  } layer_stack[CM_LAYER_MAX];
  uint32_t layer_count = 0;

  cm_renderer2d_init();
  cm_renderer_set_clear_color((vec4){1.F, 0.F, 1.F, 1.F});

  for (size_t i = 0; i < CM_LAYER_MAX && config->layers[i] != NULL; ++i) {
    layer_stack[i].interface = config->layers[i]();
    layer_stack[i].layer.app = app;
    layer_count++;
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layer_stack[i].interface.init(&layer_stack[i].layer);
  }

  float time = 0.F;
  float time_last_frame = 0.F;
  float deltatime = 0.F;
  while (app->run) {
    time = cm_window_time();
    deltatime = time - time_last_frame;
    time_last_frame = time;

    cm_renderer_clear();

    for (size_t i = 0; i < layer_count; ++i) {
      cm_camera_update(&layer_stack[i].layer.camera);
      layer_stack[i].interface.update(&layer_stack[i].layer, deltatime);
    }

    cm_window_update(app->window);
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layer_stack[i].interface.free(&layer_stack[i].layer);
  }

  cm_sound_shutdown();
  cm_renderer2d_shutdown();
}
