#include "claymore/entrypoint.h"

CmSceneInterface *sandbox(void);

static void _event(CmScene *scene, CmEvent *event) {
  (void)scene;
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_Escape) {
      cm_app_set_main(sandbox);
    }
  });
}

CmSceneInterface *manager(void) {
  static CmSceneInterface interface = {
      .event = _event,
  };
  return &interface;
}

CmSceneInterface *test(void);
CmSceneInterface *hello(void);
CmSceneInterface *benchmark(void);

static const struct {
  Str label;
  CmSceneInit interface;
} buttons[] = {
    {STR_STATIC("test"), test},
    {STR_STATIC("hello"), hello},
    {STR_STATIC("benchmark"), benchmark},
};

typedef struct {
  CmCamera2D camera;
  bool pressed;
  CmFont *font;
} Menu;
static Menu menu = {0};

static bool button(Str label, const vec2 pos, const vec2 size) {
  vec4 color = {.2f, .2f, .2f, 1};

  RGFW_window *w = cm_app_window();

  if (glm_aabb2d_point(
          (vec2[]){
              {pos[0], pos[1]},
              {pos[0] + size[0], pos[1] + size[1]},
          },
          (vec2){
              w->event.point.x,
              w->event.point.y,
          })) {
    if (!menu.pressed && RGFW_isMousePressed(w, RGFW_mouseLeft)) {
      return true;
    }
    glm_vec4_scale(color, 2, color);
  }

  cm_quad(pos, size, 0, color);
  cm_font(menu.font, (vec2){pos[0] + 20.f, pos[1]}, label);
  return false;
}

static void event(CmScene *scene, CmEvent *event) {
  (void)scene;
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_Escape) {
      cm_app_quit();
    }
  });
}

static void init(CmScene *scene) {
  cm_camera2D_screen(&menu.camera);
  menu.font = cm_font_from_file(&scene->gpu, STR("assets/fonts/Ubuntu.ttf"), 24,
                                ErrPanic);

  const vec3 bg_color = {0.05f, 0.05f, 0.05f};
  cm_app_background(bg_color);
}

static void frame_update(CmScene *scene) {
  (void)scene;
  RGFW_window *w = cm_app_window();

  cm_2D_begin(&menu.camera);
  {
    const vec2 button_size = {300, 35};
    vec2 pos = {w->r.w / (float)2 - button_size[0] / 2, 50.f};
    const float margin = 10.f;

    for (usize i = 0; i < ARRAY_LEN(buttons); ++i) {
      if (button(buttons[i].label, pos, button_size)) {
        CmScene *m = cm_app_set_main(buttons[i].interface);
        cm_scene_push(m, manager);
      }
      pos[1] += button_size[1] + margin;
    }

    if (button(STR("quit"), pos, button_size)) {
      cm_app_quit();
    }
  }
  cm_2D_end();

  menu.pressed = RGFW_isMousePressed(w, RGFW_mouseLeft);
}

CmSceneInterface *sandbox(void) {
  static CmSceneInterface interface = {
      .init = init,
      .event = event,
      .frame_update = frame_update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Sandbox"},
      .root = sandbox,
  };
  return &config;
}
