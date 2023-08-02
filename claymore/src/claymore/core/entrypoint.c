#include "cm.h"

#include "GLFW/glfw3.h"
#include "claymore/core/app.h"
#include "claymore/logger/logger.h"

#define _CM_RENDERER_INTERNAL
#include "claymore/renderer/renderer2D.h"

int main(void) {

  ClaymoreConfig config = claymore_config();

  CmApp app = {0};

  CmLayer layers[CM_LAYER_MAX];
  uint32_t layer_count = 0;

  if (!cm_app_init(&app, &config)) {
    return -1;
  }
  if (app.window == NULL) {
    return -1;
  }

  cm_renderer_init();

  for (size_t i = 0; i < CM_LAYER_MAX; ++i) {
    if (config.layers[i] == NULL) {
      break;
    }
    layers[i] = config.layers[i]();
    layers[i].data.app = &app;
    layer_count++;
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layers[i].init(&layers[i].data);
  }

  float time = 0.F;
  float time_last_frame = 0.F;
  float deltatime = 0.F;
  while (app.run) {
    time = cm_window_time();
    deltatime = time - time_last_frame;
    time_last_frame = cm_window_time();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < layer_count; ++i) {
      layers[i].update(&layers[i].data, deltatime);
    }

    cm_window_update(app.window);
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layers[i].free(&layers[i].data);
  }

  cm_renderer_shutdown();

  cm_app_shutdown(&app);

  cm_window_shutdown();
  return 0;
}
