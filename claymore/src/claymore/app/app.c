#include "app.h"

#include "scene.h"

#include "claymore/renderer/context.h"
#include "sound.h"
#include "window.h"

#include "GL/glew.h"

static double dt_get(double *last_frame) {
  double current_time = cm_window_time();
  double dt = current_time - *last_frame;
  *last_frame = current_time;
  return dt;
}

static struct {
  double last_frame;
  CmSceneInternal main_scene;
  Arena arena;
} app;

CmScene *app_root(void) { return &app.main_scene.data; }

bool app_internal_init(const ClaymoreConfig *config) {
  if (!cm_window_internal_create(config->window.width, config->window.height,
                                 config->window.title)) {
    clib_log_error("Could not open window");
    return false;
  }

  if (!cm_sound_internal_init()) {
    clib_log_error("Could not initialize sound engine");
    return false;
  }

  if (!cm_platform_context_init(cm_window_context())) {
    clib_log_error("Could not initialize context");
    return false;
  }

  app.main_scene = cm_scene_internal_init(config->scene);
  app.last_frame = cm_window_time();

  return true;
}

bool app_internal_update(void) {
  if (cm_window_internal_should_close()) {
    return false;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  double deltatime = dt_get(&app.last_frame);

  cm_scene_internal_update(&app.main_scene, deltatime);

  cm_window_internal_swap_buffers();
  cm_window_internal_poll_events();
  return true;
}

void app_internal_terminate(void) {
  cm_scene_internal_free(&app.main_scene);

  cm_sound_interal_shutdown();

  cm_window_internal_close();
  arena_free(&app.arena);
}

void app_internal_on_event(CmEvent *event) {
  cm_scene_internal_on_event(&app.main_scene, event);
}
