#include "claymore.h"

static CmFont *font;
const float font_size = 24.F;

static void window_resize_callback(CmWindowEvent *event, CmLayer *layer) {
  (void)event;
  glm_ortho(0.F, (float)layer->app->window->width, 0.F,
            (float)layer->app->window->height, -1.F, 100.F,
            layer->camera.projection);
  layer->camera.update = true;
}

static void fps_init(CmLayer *layer) {
  (void)layer;
  font = cm_font_init("res/fonts/Ubuntu.ttf", font_size);
  glm_ortho(0.F, (float)layer->app->window->width, 0.F,
            (float)layer->app->window->height, -1.F, 100.F,
            layer->camera.projection);

  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)window_resize_callback, layer);
}

static void fps_update(CmLayer *layer, float dt) {
  (void)layer, (void)dt;
  glDisable(GL_DEPTH_TEST);

  mat4 mvp;
  mat4 model;
  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  const float fps_y = layer->app->window->height - font_size;
  const float fps_x = 10.F;
#define FPS_MAX 64
  char fps_buffer[FPS_MAX] = {0};
  size_t len = snprintf(fps_buffer, FPS_MAX - 1, "%.0f FPS (%.1fms) ", 1 / dt,
                        dt * 1000);
  cm_font_draw(font, mvp, fps_x, fps_y, 0, len, fps_buffer);
  glEnable(GL_DEPTH_TEST);
}

static void fps_free(CmLayer *layer) {
  (void)layer;
  cm_font_free(font);
}

CmLayerInterface sandbox_fps(void) {
  return (CmLayerInterface){
      .init = fps_init,
      .free = fps_free,
      .update = fps_update,
  };
}
