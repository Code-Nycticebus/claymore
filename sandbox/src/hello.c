#include "claymore/entrypoint.h"

const float font_size = 48.f;

typedef struct {
  CmFont *font;
  CmCamera2D camera;
} HelloWorld;

static void init(CmScene *scene) {
  HelloWorld *hello = cm_scene_set_data(scene, sizeof(HelloWorld));

  hello->font = cm_font_init(&scene->gpu, STR("assets/fonts/Silkscreen.ttf"),
                             font_size, ErrPanic);

  cm_camera2D_screen(&hello->camera);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  HelloWorld *hello = scene->data;

  RGFW_window *window = cm_app_window();
  vec2 center;
  glm_vec2_divs((vec2){window->r.w, window->r.h}, 2, center);

  Str label = STR("Hello World");
  const vec2 pos = {
      center[0] - (label.len / 2.f) * (font_size / 2),
      center[1] - font_size,
  };
  cm_2D_begin(&hello->camera);
  { cm_font(hello->font, pos, label); }
  cm_2D_end();
}

static CmSceneInterface *hello(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Hello World"},
      .main = hello,
  };
  return &config;
}
