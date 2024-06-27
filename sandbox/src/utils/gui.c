#include "gui.h"

typedef struct {
  CmCamera2D camera;
  bool pressed;
  usize idx;
  usize selected;
  usize max;
  CmFont *font;
  vec2 size;
  vec2 anchor;
  vec2 pos;
  float margin;
  Update update;
  bool hovering;
} Gui;

static void event(CmScene *scene, CmEvent *event) {
  Gui *gui = cm_scene_data(scene);
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Up || key->code == RGFW_k) {
        gui->selected = glm_clamp(gui->selected - 1, 1, gui->max);
      }
      if (key->code == RGFW_Down || key->code == RGFW_j) {
        gui->selected = glm_clamp(gui->selected + 1, 1, gui->max);
      }
    }
  });

  cm_event_resize(event, { cm_camera2D_screen(&gui->camera); });
}

static void init(CmScene *scene) {
  Gui *gui = cm_scene_data(scene);
  cm_camera2D_screen(&gui->camera);

  Str font = STR("assets/fonts/Ubuntu.ttf");
  const float height = 24.f;
  gui->font = cm_font_from_file(&scene->gpu, font, height, ErrPanic);

  gui->margin = 10.f;
}

static void frame_update(CmScene *scene) {
  Gui *gui = cm_scene_data(scene);

  glm_vec2_copy(gui->anchor, gui->pos);
  gui->idx = 0;
  gui->hovering = false;

  cm_2D_begin(&gui->camera);
  gui->update(scene);
  cm_2D_end();

  RGFW_window *w = cm_app_window();
  gui->pressed =
      RGFW_isMousePressed(w, RGFW_mouseLeft) || RGFW_isPressedI(w, RGFW_Return);
  gui->max = gui->idx;

  if (gui->hovering) {
    RGFW_window_setMouseStandard(w, RGFW_MOUSE_POINTING_HAND);
  } else {
    RGFW_window_setMouseDefault(w);
  }
}

static void final(CmScene *scene) {
  (void)scene;
  RGFW_window *w = cm_app_window();
  RGFW_window_setMouseDefault(w);
}

CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(Gui), //
      .init = init,  //
      .frame_update = frame_update,
      .event = event,
      .final = final,
  };
  return &interface;
}

CmScene *gui_init(CmScene *parent, vec2 anchor, float width, Update update) {
  CmScene *scene = cm_scene_push(parent, interface);
  Gui *gui = cm_scene_data(scene);
  gui->size[0] = width;
  gui->size[1] = 35;
  glm_vec2_copy(anchor, gui->anchor);
  gui->update = update;
  return scene;
}

bool button(CmScene *scene, Str label) {
  Gui *gui = cm_scene_data(scene);
  RGFW_window *w = cm_app_window();
  vec4 color = {.25f, .25f, .25f, 1};
  gui->idx++;

  bool pressed = false;

  bool selected = gui->idx == gui->selected;

  if (glm_aabb2d_point(
          (vec2[]){
              {gui->pos[0], gui->pos[1]},
              {gui->pos[0] + gui->size[0], gui->pos[1] + gui->size[1]},
          },
          (vec2){w->event.point.x, w->event.point.y})) {
    if (!gui->pressed && RGFW_isMousePressed(w, RGFW_mouseLeft)) {
      pressed = true;
    }
    selected = true;
    gui->selected = 0;
    gui->hovering = true;
  }

  if (RGFW_isPressedI(w, RGFW_1 + gui->idx - 1)) {
    gui->selected = gui->idx;
  }

  if (!gui->pressed && selected && RGFW_isPressedI(w, RGFW_Return)) {
    pressed = true;
  }

  if (selected) {
    glm_vec4_scale(color, 2, color);
  }

  cm_quad(gui->pos, gui->size, 0, color);
  const float padding = 20.f;
  cm_font(gui->font, (vec2){gui->pos[0] + padding, gui->pos[1]}, label);
  gui->pos[1] += gui->size[1] + gui->margin;
  return pressed;
}
