#include "menu.h"

typedef struct {
  CmCamera2D camera;
  bool pressed;
  usize idx;
  usize selected;
  CmFont *font;
  vec2 size;
  vec2 anchor;
  vec2 pos;
  float margin;
  Update update;
} Menu;

static void event(CmScene *scene, CmEvent *event) {
  Menu *menu = cm_scene_data(scene);
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Escape) {
        cm_app_quit();
      }
      if (key->code == RGFW_Up || key->code == RGFW_k) {
        menu->selected = glm_max(menu->selected - 1, 1);
      }
      if (key->code == RGFW_Down || key->code == RGFW_j) {
        menu->selected = glm_max(menu->selected + 1, 1);
      }
    }
  });

  cm_event_resize(event, { cm_camera2D_screen(&menu->camera); });
}

static void init(CmScene *scene) {
  Menu *menu = cm_scene_data(scene);
  cm_camera2D_screen(&menu->camera);

  Str font = STR("assets/fonts/Ubuntu.ttf");
  const float height = 24.f;
  menu->font = cm_font_from_file(&scene->gpu, font, height, ErrPanic);

  menu->margin = 10.f;
}

static void pre_update(CmScene *scene) {
  Menu *menu = cm_scene_data(scene);

  glm_vec2_copy(menu->anchor, menu->pos);
  menu->idx = 0;
  cm_2D_begin(&menu->camera);
}

static void frame_update(CmScene *scene) {
  Menu *menu = cm_scene_data(scene);
  menu->update(scene);
}

static void post_update(CmScene *scene) {
  Menu *menu = cm_scene_data(scene);
  cm_2D_end();

  RGFW_window *w = cm_app_window();
  menu->pressed = RGFW_isMousePressed(w, RGFW_mouseLeft);
}

CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Menu),
      .init = init,
      .pre_update = pre_update,
      .frame_update = frame_update,
      .post_update = post_update,
      .event = event,
  };
  return &interface;
}

CmScene *menu_init(CmScene *parent, vec2 anchor, float width, Update update) {
  CmScene *scene = cm_scene_push(parent, interface);
  Menu *menu = cm_scene_data(scene);
  menu->size[0] = width;
  menu->size[1] = 35;
  glm_vec2_copy(anchor, menu->anchor);
  menu->update = update;
  return scene;
}

bool button(CmScene *scene, Str label) {
  Menu *menu = cm_scene_data(scene);
  RGFW_window *w = cm_app_window();
  vec4 color = {.25f, .25f, .25f, 1};
  menu->idx++;

  bool selected = menu->idx == menu->selected;

  if (glm_aabb2d_point(
          (vec2[]){
              {menu->pos[0], menu->pos[1]},
              {menu->pos[0] + menu->size[0], menu->pos[1] + menu->size[1]},
          },
          (vec2){w->event.point.x, w->event.point.y})) {
    if (!menu->pressed && RGFW_isMousePressed(w, RGFW_mouseLeft)) {
      return true;
    }
    selected = true;
  }

  if (RGFW_isPressedI(w, RGFW_1 + menu->idx - 1)) {
    menu->selected = menu->idx;
  }

  if (selected && RGFW_isPressedI(w, RGFW_Return)) {
    return true;
  }

  if (selected) {
    glm_vec4_scale(color, 2, color);
  }

  cm_quad(menu->pos, menu->size, 0, color);
  const float padding = 20.f;
  cm_font(menu->font, (vec2){menu->pos[0] + padding, menu->pos[1]}, label);

  menu->pos[1] += menu->size[1] + menu->margin;
  return false;
}
