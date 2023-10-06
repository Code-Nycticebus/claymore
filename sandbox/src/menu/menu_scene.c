#include "claymore.h"
static CmFont *font;
static const float font_size = 25.F;

static CmShader shader;

typedef struct {
  uint32_t scene;
  vec2s pos;
  vec2s size;
  bool selected;
  vec2s text_pos;
  const char *text;
} MenuButton;

static const char *button_labels[] = {
    "Quad Benchmark",
    "3D Cube",
    "Framebuffer",
    "Texture",
};
#define BUTTON_LABELS_COUNT (sizeof(button_labels) / sizeof(button_labels[0]))
static MenuButton buttons[BUTTON_LABELS_COUNT] = {0};
// If nothing is selected then i set it one out of bounds!
size_t buttons_selected = BUTTON_LABELS_COUNT;

const float xs = 300.F;
const float ys = 50.F;
const float margin = 20.F;
#define NORMAL_BG                                                              \
  { .2F, .2F, .2F, 1.F }
#define HIGLIGHT_BG                                                            \
  { .5F, .5F, .5F, 1.F }

static void calculate_menu_button_pos(CMwindow *window) {
  float y = window->height;
  const float x = (float)window->width / 2 - xs / 2;
  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    buttons[i].scene = i + 1;

    buttons[i].pos.x = x;

    y -= ys + margin;
    buttons[i].pos.y = y;

    buttons[i].size.x = xs;
    buttons[i].size.y = ys;

    buttons[i].selected = false;

    buttons[i].text = button_labels[i];

    buttons[i].text_pos.x = x + margin;
    buttons[i].text_pos.y = y + ys / 2 - font_size / 4;
  }
}

static bool point_in_button(const vec2s pos, MenuButton *b) {
  if (b->pos.x <= pos.x && pos.x <= b->pos.x + b->size.x) {
    if (b->pos.y <= pos.y && pos.y <= b->pos.y + b->size.y) {
      b->selected = true;
      return true;
    }
  }
  b->selected = false;
  return false;
}

static void mouse_callback(CmMouseEvent *event, void *data) {
  (void)data;
  if (event->action == CM_MOUSE_MOVE || event->action == CM_MOUSE_CLICK) {
    for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
      if (point_in_button(event->info.pos, &buttons[i])) {
        buttons_selected = i;
        if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
          cm_scene_change(buttons[i].scene);
          break;
        }
      }
    }
  }
}

static void key_callback(CmKeyEvent *event, void *data) {
  (void)data;

  if (event->action == CM_KEY_PRESS) {
    if (event->code == CM_KEY_W || event->code == CM_KEY_UP) {
      if (0 <= buttons_selected && buttons_selected < BUTTON_LABELS_COUNT) {
        buttons[buttons_selected].selected = false;
      }
      if (buttons_selected <= 0) {
        buttons_selected = BUTTON_LABELS_COUNT - 1;
      } else {
        buttons_selected -= 1;
      }
      buttons[buttons_selected].selected = true;
    }
    if (event->code == CM_KEY_S || event->code == CM_KEY_DOWN ||
        event->code == CM_KEY_TAB) {
      if (0 <= buttons_selected && buttons_selected < BUTTON_LABELS_COUNT) {
        buttons[buttons_selected].selected = false;
      }
      if (!(buttons_selected < BUTTON_LABELS_COUNT - 1)) {
        buttons_selected = 0;
      } else {
        buttons_selected += 1;
      }
      buttons[buttons_selected].selected = true;
    }
    if (event->code == CM_KEY_ENTER) {
      cm_scene_change(buttons_selected + 1);
    }
  }
}

static void window_resize_callback(CmWindowEvent *event, CmCamera *camera) {
  cm_camera_set_screen(camera, event->window->width, event->window->height);
  calculate_menu_button_pos(event->window);
}

static bool menu_init(CmScene *scene) {
  font = cm_font_init("res/fonts/Silkscreen.ttf", font_size);

  static uint32_t window_init_width = 0;
  static uint32_t window_init_height = 0;
  if (!window_init_width && !window_init_height) {
    window_init_width = scene->app->window->width;
    window_init_height = scene->app->window->height;
  } else {
    cm_window_set_size(scene->app->window, window_init_width,
                       window_init_height);
  }

  scene->camera = cm_camera_init_screen((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);

  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE,
                     (cm_event_callback)window_resize_callback, &scene->camera);
  cm_event_subscribe(CM_EVENT_MOUSE, (cm_event_callback)mouse_callback, NULL);
  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)key_callback, NULL);

  shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                    "res/shader/basic.fs.glsl");

  cm_renderer_set_clear_color((vec4s){{0, 0, 0, 1.F}});

  calculate_menu_button_pos(scene->app->window);
  buttons_selected = BUTTON_LABELS_COUNT;

  return true;
}

static void menu_update(CmScene *scene, float dt) {
  (void)dt;
  mat4s mvp = glms_mat4_mul(scene->camera.vp, glms_mat4_identity());

  cm_shader_bind(&shader);
  cm_shader_set_mat4(&shader, "u_mvp", mvp);
  cm_renderer2d_begin();
  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    cm_renderer2d_push_quad_color(buttons[i].pos, 0, buttons[i].size,
                                  buttons[i].selected ? (vec4s){HIGLIGHT_BG}
                                                      : (vec4s){NORMAL_BG});
  }
  cm_renderer2d_end();
  cm_shader_unbind();

  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    cm_font_draw_cstr(font, mvp, buttons[i].text_pos.x, buttons[i].text_pos.y,
                      1, buttons[i].text);
  }
}

static void menu_free(CmScene *scene) {
  (void)scene;
  cm_font_free(font);
  cm_shader_delete(&shader);
}

CmSceneInterface scene_menu(void) {
  return (CmSceneInterface){
      .init = menu_init,
      .free = menu_free,
      .update = menu_update,
      .layers = {0},
  };
}
