#include "claymore/core/camera.h"
#include "cm.h"

#include "claymore/core/app.h"
#include "claymore/logger/logger.h"

#define _CM_RENDERER_INTERNAL
#include "claymore/renderer/renderer.h"

int main(void) {

  const ClaymoreConfig config = claymore_config();

  CmApp app = {0};

  // Layer stack
  struct {
    CmLayerInterface interface;
    CmLayer layer;
  } layer_stack[CM_LAYER_MAX];
  uint32_t layer_count = 0;

  if (!cm_app_init(&app, &config)) {
    return -1;
  }

  cm_renderer2d_init();

  for (size_t i = 0; i < CM_LAYER_MAX && config.layers[i] != NULL; ++i) {
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

    cm_renderer_clear();

    for (size_t i = 0; i < layer_count; ++i) {
      cm_camera_update(&layer_stack[i].layer.camera);
      layer_stack[i].interface.update(&layer_stack[i].layer, deltatime);
    }

    cm_window_update(app.window);
  }

  for (size_t i = 0; i < layer_count; ++i) {
    layer_stack[i].interface.free(&layer_stack[i].layer);
  }

  cm_renderer2d_shutdown();

  cm_app_shutdown(&app);

  cm_window_shutdown();
  return 0;
}
