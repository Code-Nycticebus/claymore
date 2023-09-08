#include "app.h"

#include "claymore/core/camera.h"
#include "claymore/core/scene.h"
#include "claymore/events/event.h"
#include "claymore/logger/logger.h"
#include "claymore/sound/sound.h"
#include "cm.h"

#include "claymore/renderer/renderer.h"

#include <stdio.h>

static void _cm_app_window_close(CmWindowEvent *window, CmApp *app) {
  (void)window;
  app->run = false;
}

bool cm_app_init(CmApp *app, const ClaymoreConfig *config) {
  cm_logger_init();

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
  if (config->app.init) {
    if (!config->app.init(app)) {
      return false;
    }
  }
  cm_event_top_set(); // Registers all events as app level

  cm_renderer2d_init();
  cm_renderer_set_clear_color((vec4){1.F, 0.F, 1.F, 1.F});

  if (!cm_scene_init(app, config)) {
    return false;
  }

  return true;
}

void cm_app_shutdown(CmApp *app, const ClaymoreConfig *config) {
  cm_scene_free();
  cm_sound_shutdown();
  cm_renderer2d_shutdown();

  if (config->app.free) {
    config->app.free(app);
  }
  cm_window_close(app->window);
}

void cm_app_run(CmApp *app) {
  float time = 0.F;
  float time_last_frame = 0.F;
  float deltatime = 0.F;
  (void)deltatime;
  while (app->run) {
    time = cm_window_time();
    deltatime = time - time_last_frame;
    time_last_frame = time;

#ifdef CM_DEBUG
    static bool performace_issue_last_frame = false;
    const float min_dt = 1 / 59.F;
    if (min_dt < deltatime) {
      if (performace_issue_last_frame) {
        cm_log_warning("Performance warning: %.0F FPS\n", 1 / deltatime);
        performace_issue_last_frame = false;
      } else {
        performace_issue_last_frame = true;
      }
    } else {
      performace_issue_last_frame = false;
    }
#endif

    cm_renderer_clear();

    cm_scene_update(deltatime);

    cm_window_update(app->window);
  }
}
