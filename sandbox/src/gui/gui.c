#include "gui.h"

typedef struct {
  vec2s pos;
  vec2s size;
  vec4s color;
} Quad;

void quad_push(const Quad *quad) {
  cm_renderer2d_push_quad_color(quad->pos, 0, quad->size, quad->color);
}

typedef struct {
  CmShader shader;
  Quad quad;
} BgData;

static bool bg_layer_init(CmScene *scene, CmLayer *layer) {
  BgData *data = malloc(sizeof(BgData));
  data->quad.pos = (vec2s){0};
  data->quad.size =
      (vec2s){{scene->app->window->width, scene->app->window->height}};

  data->shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  layer->state = data;
  return true;
}

static void bg_layer_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)dt;
  BgData *data = layer->state;
  vec4s *scene_color = scene->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_mvp", scene->camera.vp);

  data->quad.color = *scene_color;
  cm_renderer2d_begin();
  quad_push(&data->quad);
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
  Quad button;
  Quad bg;
  Quad rail;
  float *value;
} Slider;

const vec4s slider_bg_color = {{.2F, .2F, .2F, 1.F}};

Slider slider(vec2s pos, vec2s size) {
  Slider slider;
  slider.bg.pos = pos;
  slider.bg.size = size;
  slider.bg.color = slider_bg_color;
  return slider;
}

void slider_render(Slider *slider) { quad_push(&slider->bg); }

typedef struct {
  CmShader shader;
  Slider slider;
} OverlayData;

static bool overlay_init(CmScene *scene, CmLayer *layer) {
  (void)scene;
  OverlayData *data = malloc(sizeof(OverlayData));

  data->shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                          "res/shader/basic.fs.glsl");

  data->slider = slider((vec2s){{10.F, 10.F}}, (vec2s){{100.F, 200.F}});

  layer->state = data;
  return true;
}

void overlay_update(CmScene *scene, CmLayer *layer, float dt) {
  (void)scene, (void)layer, (void)dt;
  OverlayData *data = layer->state;

  cm_shader_bind(&data->shader);
  cm_shader_set_mat4(&data->shader, "u_mvp", scene->camera.vp);

  cm_renderer2d_begin();
  slider_render(&data->slider);
  cm_renderer2d_end();

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
  vec4s *color = malloc(sizeof(vec4s));
  *color = (vec4s){{1, 0, 0, 1}};
  scene->state = color;

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
              bg_layer,
              overlay_layer,
              NULL,
          },
  };
}
