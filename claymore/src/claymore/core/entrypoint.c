#include "cm.h"

#include "GLFW/glfw3.h"
#include "claymore/core/app.h"
#include "claymore/logger/logger.h"

#define _CM_RENDERER_INTERNAL
#include "claymore/renderer/renderer2D.h"

int main(void) {

  const ClaymoreConfig config = claymore_config();

  CmApp app = {0};

  // Layer stack
  struct {
    CmLayerInterface interface;
    CmLayer layer;
  } layer_stack[CM_LAYER_COUNT];
  uint32_t layer_count = 0;

  if (!cm_app_init(&app, &config)) {
    return -1;
  }

  cm_renderer_init();

  for (size_t i = 0; i < CM_LAYER_COUNT && config.layers[i] != NULL; ++i) {
    layer_stack[i].interface = config.layers[i]();
    layer_stack[i].layer.app = &app;
    layer_count++;
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layer_stack[i].interface.init(&layer_stack[i].layer);
  }

  float time = 0.F;
  float time_last_frame = 0.F;
  float deltatime = 0.F;
  while (app.run) {
    time = cm_window_time();
    deltatime = time - time_last_frame;
    time_last_frame = time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < layer_count; ++i) {
      layer_stack[i].interface.update(&layer_stack[i].layer, deltatime);
    }

    cm_window_update(app.window);
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layer_stack[i].interface.free(&layer_stack[i].layer);
  }

  cm_renderer_shutdown();

  cm_app_shutdown(&app);

  cm_window_shutdown();
  return 0;
}
