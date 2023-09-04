#include "claymore.h"
#include "claymore/core/font.h"
#include "claymore/renderer/renderer2D.h"

static CmFont *font;
static const float font_size = 25.F;

static CmShader shader;

typedef struct {
  uint32_t scene;
  vec2 pos;
  vec2 size;
  bool selected;
  vec2 text_pos;
  const char *text;
} MenuButton;

static const char *button_labels[] = {
    "Quad Benchmark",
    "3D Cube",
    "Framebuffer",
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
  const float x = window->width / 2 - xs / 2;
  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    buttons[i].scene = i + 1;

    buttons[i].pos[0] = x;

    y -= ys + margin;
    buttons[i].pos[1] = y;

    buttons[i].size[0] = xs;
    buttons[i].size[1] = ys;

    buttons[i].selected = false;

    buttons[i].text = button_labels[i];

    buttons[i].text_pos[0] = x + margin;
    buttons[i].text_pos[1] = y + ys / 2 - font_size / 4;
  }
}

static bool point_in_button(const vec2 pos, MenuButton *b) {
  if (b->pos[0] <= pos[0] && pos[0] <= b->pos[0] + b->size[0]) {
    if (b->pos[1] <= pos[1] && pos[1] <= b->pos[1] + b->size[1]) {
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
    if (event->code == CM_KEY_S || event->code == CM_KEY_DOWN) {
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

static void window_resize_callback(CmWindowEvent *event, CmLayer *layer) {
  (void)event;
  glm_ortho(0.F, (float)event->window->width, 0.F, (float)event->window->height,
            -1.F, 100.F, layer->camera.projection);
  layer->camera.update = true;

  calculate_menu_button_pos(event->window);
}

static bool menu_init(CmScene *scene, CmLayer *layer) {
  (void)layer;
  font = cm_font_init("res/fonts/Silkscreen.ttf", font_size);
  glm_ortho(0.F, (float)scene->app->window->width, 0.F,
            (float)scene->app->window->height, -1.F, 100.F,
            layer->camera.projection);

  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)window_resize_callback, layer);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)mouse_callback,
                        NULL);
  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)key_callback,
                        NULL);
  shader = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                    "res/shader/basic.fs.glsl");

  cm_renderer_set_clear_color((vec4){0, 0, 0, 1.F});
  calculate_menu_button_pos(scene->app->window);
  return true;
}

static void menu_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)layer, (void)dt, (void)scene;

  mat4 mvp;
  mat4 model;
  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  cm_shader_bind(&shader);
  cm_shader_set_mat4(&shader, "u_mvp", mvp);

  cm_renderer2d_begin();
  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    cm_renderer2d_push_quad_color(buttons[i].pos, 0, buttons[i].size,
                                  buttons[i].selected ? (vec4)HIGLIGHT_BG
                                                      : (vec4)NORMAL_BG);
  }
  cm_renderer2d_end();

  glUseProgram(0);

  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    cm_font_draw_cstr(font, mvp, buttons[i].text_pos[0], buttons[i].text_pos[1],
                      1, buttons[i].text);
  }
}

static void menu_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
  cm_font_free(font);
  cm_shader_delete(&shader);
}

CmLayerInterface sandbox_menu(void) {
  return (CmLayerInterface){
      .init = menu_init,
      .free = menu_free,
      .update = menu_update,
  };
}
