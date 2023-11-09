#include "gui.h"
#include "claymore/renderer/font.h"

typedef struct {
  vec2s pos;
  vec2s size;
  vec4s color;
} Quad;

bool quad_collide_pos(const Quad *q1, const vec2s pos) {
  // Check if the input position is within the boundaries of the Quad
  if (pos.x >= q1->pos.x && pos.x <= q1->pos.x + q1->size.x &&
      pos.y >= q1->pos.y && pos.y <= q1->pos.y + q1->size.y) {
    return true; // Collision occurred
  }
  return false; // No collision
}

void quad_push(const Quad *quad) {
  cm_renderer2d_push_quad_color(quad->pos, 0, quad->size, quad->color);
}

typedef struct {
  Quad quad;
} SceneData;

typedef struct {
  CmShader shader;
} BgData;

static bool bg_layer_init(CmScene *scene, CmLayer *layer) {
  (void)scene;
  BgData *data = malloc(sizeof(BgData));

  data->shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  layer->state = data;
  return true;
}

static void bg_layer_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)dt;
  BgData *data = layer->state;
  SceneData *scene_data = scene->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_mvp", scene->camera.vp);

  cm_renderer2d_begin();
  quad_push(&scene_data->quad);
  cm_renderer2d_end();

  cm_shader_unbind();
}

static void bg_layer_free(CmScene *scene, CmLayer *layer) {
  (void)scene;
  free(layer->state);
}

static CmLayerInterface bg_layer(void) {
  return (CmLayerInterface){
      .free = bg_layer_free,
      .update = bg_layer_update,
      .init = bg_layer_init,
  };
}

typedef struct {
  bool active;
  Quad button;
  Quad bg;
  Quad rail;
  float min;
  float max;
  float *value;
} Slider;

const vec4s slider_bg_color = {{.2F, .2F, .2F, 1.F}};
const vec4s slider_rail_color = {{.3F, .3F, .3F, 1.F}};
const vec4s slider_button_color = {{.4F, .4F, .4F, 1.F}};

Slider slider(vec2s pos, vec2s size, float min, float max, float *value) {
  Slider slider;
  slider.active = false;
  slider.bg.pos = pos;
  slider.bg.size = size;
  slider.bg.color = slider_bg_color;

  const vec2s rail_size = (vec2s){{size.x * 0.85F, 5}};
  slider.rail.size = rail_size;
  const vec2s rail_pos = (vec2s){{(pos.x + size.x / 2.F) - rail_size.x / 2.F,
                                  (pos.y + size.y / 2.F) - rail_size.y / 2.F}};
  slider.rail.pos = rail_pos;
  slider.rail.color = slider_rail_color;

  slider.button.color = slider_button_color;
  const vec2s button_size = (vec2s){{10, 20}};
  slider.button.size = button_size;
  const vec2s button_pos =
      (vec2s){{(rail_pos.x + rail_size.x / 2.F) - button_size.x / 2.F,
               (rail_pos.y + rail_size.y / 2.F) - button_size.y / 2.F}};
  slider.button.pos = button_pos;

  slider.max = max;
  slider.min = min;
  slider.value = value;

  float progress = *value > 0 ? max / *value : 0;
  slider.button.pos.x =
      glm_lerp(slider.rail.pos.x, slider.rail.pos.x + slider.rail.size.x,
               progress) -
      (slider.button.size.x / 2);
  return slider;
}

typedef struct {
  size_t cap;
  size_t len;
  Slider *data;
} SliderStack;

SliderStack slider_stack_allocate(size_t cap) {
  SliderStack stack = {0};
  stack.cap = cap;
  stack.data = malloc(cap * sizeof(stack.data[0]));
  assert(stack.data);
  return stack;
}

void slider_stack_push(SliderStack *stack, Slider slider) {
  if (!(stack->len < stack->cap)) {
    stack->cap = (stack->cap + 1) * 2;
    stack->data = realloc(stack->data, stack->cap * sizeof(stack->data[0]));
  }
  stack->data[stack->len++] = slider;
}

void slider_stack_free(SliderStack *stack) {
  free(stack->data);
  stack->data = NULL;
  stack->cap = 0;
  stack->len = 0;
}

void slider_render(SliderStack *sliders) {
  cm_renderer2d_begin();
  for (size_t i = 0; i < sliders->len; i++) {
    Slider *slider = &sliders->data[i];
    quad_push(&slider->bg);
    quad_push(&slider->rail);
    quad_push(&slider->button);
  }
  cm_renderer2d_end();
}

typedef struct {
  CmShader shader;
  SliderStack slider_stack;
} OverlayData;

static void slider_callback(CmMouseEvent *event, SliderStack *sliders) {
  if (event->action == CM_MOUSE_CLICK) {
    for (size_t i = 0; i < sliders->len; i++) {
      Slider *slider = &sliders->data[i];
      if (quad_collide_pos(&slider->bg, event->info.pos)) {
        slider->active = true;
      } else {
        slider->active = false;
      }
    }
  }
  if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
    for (size_t i = 0; i < sliders->len; i++) {
      Slider *slider = &sliders->data[i];
      if (slider->active) {
        float height = event->info.pos.x - slider->rail.pos.x;
        float progress = height / slider->rail.size.x;
        *slider->value = glm_clamp(glm_lerp(slider->min, slider->max, progress),
                                   slider->min, slider->max);
        slider->button.pos.x =
            glm_clamp(
                glm_lerp(slider->rail.pos.x,
                         slider->rail.pos.x + slider->rail.size.x, progress),
                slider->rail.pos.x, slider->rail.pos.x + slider->rail.size.x) -
            (slider->button.size.x / 2);
      }
    }
  }
}

static void slider_key_callback(CmKeyEvent *event, SliderStack *sliders) {
  Slider *slider = NULL;
  for (size_t i = 0; i < sliders->len; i++) {
    if (sliders->data[i].active) {
      slider = &sliders->data[i];
    }
  }
  if (slider == NULL) {
    return;
  }

  if (event->action == CM_KEY_PRESS || event->action == CM_KEY_REPEAT) {
    const float step = 0.1F;
    float progress = *slider->value / slider->max;
    if (event->code == CM_KEY_RIGHT) {
      progress += step;
    } else if (event->code == CM_KEY_LEFT) {
      progress -= step;
    }
    *slider->value = glm_clamp(glm_lerp(slider->min, slider->max, progress),
                               slider->min, slider->max);
    slider->button.pos.x =
        glm_clamp(glm_lerp(slider->rail.pos.x,
                           slider->rail.pos.x + slider->rail.size.x, progress),
                  slider->rail.pos.x,
                  slider->rail.pos.x + slider->rail.size.x) -
        (slider->button.size.x / 2);
  }
}

static bool overlay_init(CmScene *scene, CmLayer *layer) {
  OverlayData *data = malloc(sizeof(OverlayData));
  SceneData *scene_data = scene->state;

  data->shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");
  const vec2s slider_size = {{150, 30}};
  const float margin = 5.F;
  data->slider_stack = slider_stack_allocate(3);

  slider_stack_push(
      &data->slider_stack,
      slider((vec2s){{margin, (slider_size.y * 0) + (margin * 1)}}, slider_size,
             0, 1, &scene_data->quad.color.r));
  slider_stack_push(
      &data->slider_stack,
      slider((vec2s){{margin, (slider_size.y * 1) + (margin * 2)}}, slider_size,
             0, 1, &scene_data->quad.color.g));
  slider_stack_push(
      &data->slider_stack,
      slider((vec2s){{margin, (slider_size.y * 2) + (margin * 3)}}, slider_size,
             0, 1, &scene_data->quad.color.b));

  slider_stack_push(
      &data->slider_stack,
      slider((vec2s){{margin, (slider_size.y * 3) + (margin * 4)}}, slider_size,
             0, scene->app->window->width - scene_data->quad.size.x,
             &scene_data->quad.pos.x));
  slider_stack_push(
      &data->slider_stack,
      slider((vec2s){{margin, (slider_size.y * 4) + (margin * (4 + 1))}},
             slider_size, 0,
             scene->app->window->height - scene_data->quad.size.y,
             &scene_data->quad.pos.y));

  cm_event_subscribe(CM_EVENT_MOUSE, (cm_event_callback)slider_callback,
                     &data->slider_stack);
  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)slider_key_callback,
                     &data->slider_stack);

  layer->state = data;
  return true;
}

void overlay_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)scene, (void)layer, (void)dt;
  OverlayData *data = layer->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_mvp", scene->camera.vp);

  slider_render(&data->slider_stack);

  cm_shader_unbind();
}

void overlay_free(CmScene *scene, CmLayer *layer) {
  (void)scene;
  free(layer->state);
}

static CmLayerInterface overlay_layer(void) {
  return (CmLayerInterface){
      .free = overlay_free,
      .update = overlay_update,
      .init = overlay_init,
  };
}

bool gui_init(CmScene *scene) {
  SceneData *data = malloc(sizeof(SceneData));
  data->quad = (Quad){
      .size = {{100.F * 3, 100.F * 3}},
      .pos = {{0, 0}},
      .color = {{1, 0, 0, 1}},

  };
  scene->state = data;

  scene->camera = cm_camera_init_screen((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);
  glDisable(GL_DEPTH_TEST);
  return true;
}

void gui_free(CmScene *scene) {
  glEnable(GL_DEPTH_TEST);
  free(scene->state);
}

CmSceneInterface gui_scene(void) {
  return (CmSceneInterface){
      .init = gui_init,
      .free = gui_free,
      .update = NULL,
      .layers =
          {
              {bg_layer, true},
              {overlay_layer, true},
              {0},
          },
  };
}
