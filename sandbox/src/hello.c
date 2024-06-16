#include "claymore/entrypoint.h"
#include "utils/frame_count.h"

const Str font = STR_STATIC("assets/fonts/Silkscreen.ttf");
const float font_size = 48.f;

const Str label = STR_STATIC("Hello World");

typedef struct {
  CmFont *font;
  CmCamera2D camera;
  vec2 position;
} HelloWorld;

static void init(CmScene *scene) {
  // frame_count(scene, STR(".build/hello-optimize.csv"));
  HelloWorld *hello = cm_scene_set_data(scene, sizeof(HelloWorld));
  hello->font = cm_font_from_file(&scene->gpu, font, font_size, ErrPanic);
  cm_camera2D_screen(&hello->camera);

  RGFW_window *window = cm_app_window();
  vec2 center = {window->r.w / (float)2, window->r.h / (float)2};
  hello->position[0] = center[0] - (label.len / (float)2) * (font_size / 2);
  hello->position[1] = center[1] - font_size;
}

static void frame_update(CmScene *scene) {
  HelloWorld *hello = scene->data;

  cm_2D_begin(&hello->camera);
  { cm_font(hello->font, hello->position, label); }
  cm_2D_end();
}

static CmSceneInterface *hello(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Hello World"},
      .root = hello,
  };
  return &config;
}
