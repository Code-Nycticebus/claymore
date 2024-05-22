#include "shader_toy.h"

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  Str message;
} ErrorDisplay;

static void init(CmScene *scene) {
  ErrorDisplay *display = cm_scene_alloc_data(scene, sizeof(ErrorDisplay));
  display->font =
      cm_font_init(&scene->gpu, STR("assets/fonts/Ubuntu.ttf"), 20.f, ErrPanic);

  cm_camera2d_screen(&display->camera);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  ErrorDisplay *display = scene->data;

  cm_renderer2d_begin(&display->camera);
  cm_font(display->font, (vec2){10, 10}, display->message);
  cm_renderer2d_end();
}

static CmSceneInterface *error_display(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
  };
  return &interface;
}

CmScene *error_display_init(CmScene *parent, Str message) {
  CmScene *scene = cm_scene_push(parent, error_display);
  ErrorDisplay *display = scene->data;
  display->message = message;
  return scene;
}
