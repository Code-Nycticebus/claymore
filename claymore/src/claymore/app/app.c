#include "app.h"

#include "claymore/renderer/context.h"
#include "window.h"

#include "GL/glew.h"

static double dt_get(double *last_frame) {
  double current_time = cm_window_time();
  double dt = current_time - *last_frame;
  *last_frame = current_time;
  return dt;
}

typedef struct {
  double last_frame;
  CmSceneInternal main_scene;
} CmApp;

static CmApp app;

bool app_init(const ClaymoreConfig *config) {
  if (!cm_window_create(config->window.width, config->window.height,
                        config->window.title)) {
    clib_log_error("Could not open window");
    return false;
  }

  cm_platform_context_init(cm_window_context());

  glClearColor(1, 0, 1, 1);

  app.main_scene = cm_scene_internal_init(config->scene);

  app.last_frame = cm_window_time();

  return true;
}

bool app_update(void) {
  if (cm_window_should_close()) {
    return false;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  double deltatime = dt_get(&app.last_frame);

  // RENDER HERE
  cm_scene_internal_update(&app.main_scene, deltatime);

  cm_window_swap_buffers();
  cm_window_poll_events();
  return true;
}

void app_terminate(void) {
  cm_scene_internal_free(&app.main_scene);

  cm_window_close();
}
