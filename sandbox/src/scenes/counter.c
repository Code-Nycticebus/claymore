#include "claymore.h"

#include "utils/menu.h"

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  i32 i;
  vec2 pos;
} Counter;

static void init(CmScene *scene) {
  Counter *counter = cm_scene_data(scene);

  const float font_height = 96.f;
  Str font = STR("assets/fonts/Ubuntu.ttf");
  counter->font = cm_font_from_file(&scene->gpu, font, font_height, ErrPanic);

  cm_camera2D_screen(&counter->camera);

  RGFW_window *w = cm_app_window();
  glm_vec2_divs((vec2){w->r.w, w->r.h}, 2, counter->pos);
  counter->pos[0] -= font_height / 2 * 2;
  counter->pos[1] -= font_height;
}

static void frame_update(CmScene *scene) {
  Counter *counter = cm_scene_data(scene);
  cm_2D_begin(&counter->camera);
  Str label = str_format(&scene->arena, "%*d", 4, counter->i);
  cm_font(counter->font, counter->pos, label);
  cm_2D_end();

  arena_reset(&scene->arena);
}

CmSceneInterface *_counter(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Counter),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

static void menu_update(CmScene *scene) {
  Counter *counter = cm_scene_data(cm_scene_child(cm_scene_parent(scene), 0));
  if (button(scene, STR("+"))) {
    counter->i++;
  }
  if (button(scene, STR("-"))) {
    counter->i--;
  }
}

static void test_init(CmScene *scene) {
  const float width = 50.f;

  cm_scene_push(scene, _counter);
  menu_init(scene, (vec2){25, 25}, width, menu_update);
}

CmSceneInterface *counter(void) {
  static CmSceneInterface interface = {
      .init = test_init,
  };
  return &interface;
}
