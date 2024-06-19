#include "claymore.h"

const Str label = STR_STATIC("Hello World");

typedef struct {
  CmFont *font;
  CmCamera2D camera;
  vec2 position;
} HelloWorld;

static void init(CmScene *scene) {
  HelloWorld *hello = cm_scene_set_data(scene, sizeof(HelloWorld));

  const Str font = STR_STATIC("assets/fonts/Silkscreen.ttf");
  const float font_size = 72.f;
  hello->font = cm_font_from_file(&scene->gpu, font, font_size, ErrPanic);
  cm_camera2D_screen(&hello->camera);

  RGFW_window *window = cm_app_window();
  vec2 center = {window->r.w / (float)2, window->r.h / (float)2};
  hello->position[0] = center[0] - (label.len / (float)2) * (font_size / 2);
  hello->position[1] = center[1] - font_size;
}

static void frame_update(CmScene *scene) {
  HelloWorld *hello = scene->data;

  const float r = sinf(cm_app_time() / 1e9) * 0.5f + 0.5f;
  const float b = cosf(cm_app_time() / 1e9) * 0.5f + 0.5f;
  cm_font_color(hello->font, (vec4){r, 0, b, 1.f});

  cm_2D_begin(&hello->camera);
  { cm_font(hello->font, hello->position, label); }
  cm_2D_end();
}

CmSceneInterface *hello(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}
