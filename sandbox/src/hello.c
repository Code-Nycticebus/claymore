#include "cglm/mat4.h"
#include "cglm/types.h"
#include "claymore/entrypoint.h"

typedef struct {
  CmFont *font;
  mat4 mvp;
} HelloWorld;

static void init(CmScene *scene) {
  HelloWorld *hello = cm_scene_alloc_data(scene, sizeof(HelloWorld));

  hello->font = cm_font_init(&scene->gpu, STR("res/fonts/Silkscreen.ttf"), 48.f,
                             ErrPanic);

  vec2 window_size;
  cm_window_get_size(window_size);

  mat4 p;
  glm_ortho(0, window_size[0], window_size[1], 0, -1.F, 100.F, p);

  mat4 view = GLM_MAT4_IDENTITY_INIT;
  glm_translate(view, (vec3){0});

  glm_mat4_mul(p, view, hello->mvp);
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  HelloWorld *hello = scene->data;

  vec2 center;
  cm_window_get_size(center);
  glm_vec2_divs(center, 2, center);

  Str label = STR("Hello World");
  center[0] -= (label.len / 2.f) * 24.f;
  center[1] -= 48.f;
  cm_font_draw(hello->font, hello->mvp, center, label);
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
      .window = {.width = 720, .height = 420, .title = "basic"},
      .main = hello,
  };
  return &config;
}
