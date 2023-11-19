#include "app.h"

#include "claymore/core/scene.h"
#include "claymore/events/event.h"
#include "claymore/sound/sound.h"
#include "cm.h"

#define CM_RENDERER_PRIVATE
#include "claymore/renderer/renderer.h"

static void _cm_app_window_close(CmWindowEvent *window, CmApp *app) {
  (void)window;
  app->run = false;
}

bool cm_app_init(CmApp *app, const ClaymoreConfig *config) {
  cm_logger_init();

  cm_event_subscribe(CM_EVENT_WINDOW_CLOSE,
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
  cm_renderer_lines_init();
#ifdef CM_DEBUG
  cm_renderer_set_clear_color((vec4s){{1.F, 0.F, 1.F, 1.F}});
#endif
  if (!cm_scene_init(app, config)) {
    return false;
  }

  return true;
}

void cm_app_shutdown(CmApp *app, const ClaymoreConfig *config) {
  cm_scene_free();
  if (config->app.free) {
    config->app.free(app);
  }

  cm_sound_shutdown();
  cm_renderer2d_shutdown();
  cm_renderer_lines_shutdown();
  cm_window_close(app->window);
}

void cm_app_run(CmApp *app, const ClaymoreConfig *config) {
  float time = 0.F;
  float time_last_frame = 0.F;
  float deltatime = 0.F;
  while (app->run) {
    time = cm_window_time();
    deltatime = time - time_last_frame;
    time_last_frame = time;

    cm_renderer_clear();

    if (config->app.update) {
      config->app.update(app, deltatime);
    }
    cm_scene_update(deltatime);

    cm_window_update(app->window);
  }
}
