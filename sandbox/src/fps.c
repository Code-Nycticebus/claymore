#include "claymore.h"

static CmFont *font;
const float font_size = 24.F;

static void window_resize_callback(CmWindowEvent *event, CmCamera *camera) {
  cm_camera_set_screen(camera, event->window->width, event->window->height);
}

static bool fps_init(CmScene *scene, CmLayer *layer) {
  (void)layer;
  font = cm_font_init("res/fonts/Silkscreen.ttf", font_size);

  layer->camera =
      cm_camera_init_screen((vec3s){0}, (float)scene->app->window->width,
                            (float)scene->app->window->height);

  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE,
                     (cm_event_callback)window_resize_callback, &layer->camera);
  return true;
}

static void fps_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)layer, (void)dt;
  glDisable(GL_DEPTH_TEST);

  mat4s mvp = glms_mat4_mul(layer->camera.vp, glms_mat4_identity());

  const float fps_y = scene->app->window->height - font_size;
  const float fps_x = 10.F;
#define FPS_MAX 64
  char fps_buffer[FPS_MAX] = {0};
  const uint32_t ms_per_second = 1000;
  float fps = 1 / dt;
  size_t len = snprintf(fps_buffer, FPS_MAX - 1, "  %.0f FPS (%.1fms)", fps,
                        dt * ms_per_second);
  cm_font_draw(font, mvp, fps_x, fps_y, 0, len, fps_buffer);
  glEnable(GL_DEPTH_TEST);
}

static void fps_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
  cm_font_free(font);
}

CmLayerInterface sandbox_fps(void) {
  return (CmLayerInterface){
      .init = fps_init,
      .free = fps_free,
      .update = fps_update,
  };
}
