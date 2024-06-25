#include "claymore/entrypoint.h"

#include "utils/fps.h"

CmSceneInterface *sandbox(void);

static void _event(CmScene *scene, CmEvent *event) {
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_Escape) {
      cm_app_set_root(sandbox);
      event->handled = true;
    }
  });

  cm_event_key(event, {
    if (key->action == RGFW_keyPressed && key->code == RGFW_F1) {
      CmScene *f = cm_scene_find(scene, STR("Fps"));
      if (f == NULL) {
        const vec2 pos = {10, 0};
        const Str font = STR_STATIC("assets/fonts/Ubuntu.ttf");
        const float font_size = 32.f;
        fps(scene, pos, font, font_size);
      } else {
        cm_scene_delete(f);
      }
    }
  });
}

CmSceneInterface *app_controlls(void) {
  static CmSceneInterface interface = {
      .event = _event,
  };
  return &interface;
}

CmSceneInterface *test(void);
CmSceneInterface *hello(void);
CmSceneInterface *benchmark(void);
CmSceneInterface *template(void);

static const struct {
  Str label;
  CmSceneInit scene;
} buttons[] = {
    {.label = STR_STATIC("test"), .scene = test},
    {.label = STR_STATIC("hello"), .scene = hello},
    {.label = STR_STATIC("benchmark"), .scene = benchmark},
    {.label = STR_STATIC("quit"), .scene = NULL},
};

typedef struct {
  CmCamera2D camera;
  bool pressed;
  usize selected;
  CmFont *font;
} Menu;
static Menu menu = {0};

static bool button(Str label, const vec2 pos, const vec2 size, vec4 color) {
  RGFW_window *w = cm_app_window();

  if (glm_aabb2d_point(
          (vec2[]){{pos[0], pos[1]}, {pos[0] + size[0], pos[1] + size[1]}},
          (vec2){w->event.point.x, w->event.point.y})) {
    if (!menu.pressed && RGFW_isMousePressed(w, RGFW_mouseLeft)) {
      return true;
    }
    glm_vec4_scale(color, 2, color);
  }

  cm_quad(pos, size, 0, color);
  const float padding = 20.f;
  cm_font(menu.font, (vec2){pos[0] + padding, pos[1]}, label);
  return false;
}

static void event(CmScene *scene, CmEvent *event) {
  (void)scene;
  cm_event_key(event, {
    if (key->action == RGFW_keyPressed) {
      if (key->code == RGFW_Escape) {
        cm_app_quit();
      }
      if (key->code == RGFW_Up || key->code == RGFW_k) {
        menu.selected = glm_clamp(menu.selected - 1, 1, ARRAY_LEN(buttons));
      }
      if (key->code == RGFW_Down || key->code == RGFW_j) {
        menu.selected = glm_clamp(menu.selected + 1, 1, ARRAY_LEN(buttons));
      }

      for (usize i = 0; i < ARRAY_LEN(buttons); ++i) {
        if (key->code == RGFW_1 + i) {
          menu.selected = i + 1;
        }
      }
    }
  });

  cm_event_resize(event, {
    (void)resize;
    cm_camera2D_screen(&menu.camera);
  });
}

static void init(CmScene *scene) {
  cm_camera2D_screen(&menu.camera);

  Str font = STR("assets/fonts/Ubuntu.ttf");
  const float height = 24.f;
  menu.font = cm_font_from_file(&scene->gpu, font, height, ErrPanic);

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
      vec4 color = {.2f, .2f, .2f, 1};
      if (menu.selected == i + 1) {
        glm_vec4_scale(color, 2, color);
      }
      if (button(buttons[i].label, pos, button_size, color) ||
          (menu.selected == i + 1 && RGFW_isPressedI(w, RGFW_Return))) {
        if (buttons[i].scene) {
          CmScene *m = cm_app_set_root(buttons[i].scene);
          cm_scene_push(m, app_controlls);
        } else {
          cm_app_quit();
        }
      }
      pos[1] += button_size[1] + margin;
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
