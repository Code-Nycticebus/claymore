#include "claymore.h"
#include "claymore/core/font.h"
#include "claymore/renderer/renderer2D.h"

static CmFont *font;
static const float font_size = 25.F;

struct MenuShader {
  uint32_t id;
  struct {
    uint32_t mvp;
  } loc;
};

static struct MenuShader shader;

typedef struct {
  uint32_t scene;
  vec2 pos;
  vec2 size;
  vec4 color;
  vec2 text_pos;
  const char *text;
} MenuButton;

static const char *button_labels[] = {
    "Quad Benchmark",
    "3D Cube",
};
#define BUTTON_LABELS_COUNT (sizeof(button_labels) / sizeof(button_labels[0]))
static MenuButton buttons[BUTTON_LABELS_COUNT] = {0};

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

    glm_vec4_copy((vec4)NORMAL_BG, buttons[i].color);

    buttons[i].text = button_labels[i];

    buttons[i].text_pos[0] = x + margin;
    buttons[i].text_pos[1] = y + ys / 2 - font_size / 4;
  }
}

static bool point_in_button(const vec2 pos, MenuButton *b) {
  if (b->pos[0] <= pos[0] && pos[0] <= b->pos[0] + b->size[0]) {
    if (b->pos[1] <= pos[1] && pos[1] <= b->pos[1] + b->size[1]) {
      glm_vec4_copy((vec4)HIGLIGHT_BG, b->color);
      return true;
    }
  }
  glm_vec4_copy((vec4)NORMAL_BG, b->color);
  return false;
}

static void mouse_callback(CmMouseEvent *event, void *data) {
  (void)data;
  if (event->action == CM_MOUSE_MOVE || event->action == CM_MOUSE_CLICK) {
    for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
      if (point_in_button(event->info.pos, &buttons[i])) {
        if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
          cm_scene_change(buttons[i].scene);
          break;
        }
      }
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
  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

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

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer2d_begin();
  for (size_t i = 0; i < BUTTON_LABELS_COUNT; i++) {
    cm_renderer2d_push_quad_color(buttons[i].pos, 0, buttons[i].size,
                                  buttons[i].color);
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
  glDeleteProgram(shader.id);
}

CmLayerInterface sandbox_menu(void) {
  return (CmLayerInterface){
      .init = menu_init,
      .free = menu_free,
      .update = menu_update,
  };
}
