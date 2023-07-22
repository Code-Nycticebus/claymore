
#include <stdio.h>
#include <stdlib.h>

#include "cglm/vec4.h"
#include "claymore.h"

#include "GL/gl.h"
#include "cglm/cglm.h"
#include "claymore/events/mouse.h"
#include <time.h>

const int WINDOW_WIDTH = 620;
const int WINDOW_HEIGHT = 420;
const float near = -0.1F;

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } u_loc;
};

static struct ShaderData quad_shaders;

mat4 quad_model;

typedef struct {
  vec2 size;
  vec2 pos;
  float z;
  vec4 color;
} Quad2D;

#define MAX_QUADS 2000
static const uint32_t quad_size = 15;
static uint32_t quad_count;
Quad2D quads[MAX_QUADS];

#define TOP_BAR_COLOR_COUNT 10
const float top_bar_height = 50.F;
const float top_bar_color_quad_width = WINDOW_WIDTH / TOP_BAR_COLOR_COUNT;
Quad2D top_bar_colors[TOP_BAR_COLOR_COUNT];

static vec4 quad_color;
static float layer;
static const float layer_inc = 0.1F;

static void _sandbox_controll(CmApp *app, CmKeyEvent *event) {
  (void)app;
  if (event->action == CM_KEY_PRESS && event->code == CM_KEY_F5) {
    quad_count = 0;
    event->base.handled = true;
  }
}

static bool _sandbox_point_in_quad(const vec2 point, const Quad2D *quad) {
  const float fraction_xs = 1.F;
  const float franction_ys = 1.F;
  return (
      (point[0] > quad->pos[0] + fraction_xs) &&
      (point[0] < quad->pos[0] + quad->size[0] - fraction_xs) // if in x axis
      &&                                                      // and
      (point[1] > quad->pos[1] + franction_ys) &&             // if in y axis
      (point[1] < quad->pos[1] + quad->size[1] - franction_ys));
}

static void _sandbox_mouse(CmApp *app, CmMouseEvent *event) {
  (void)event, (void)app;
  if (event->action == CM_MOUSE_CLICK) {
    layer += layer_inc;
    for (size_t i = 0; i < TOP_BAR_COLOR_COUNT; ++i) {
      Quad2D *quad = &top_bar_colors[i];
      if (_sandbox_point_in_quad(event->info.pos, quad)) {
        glm_vec4_copy(quad->color, quad_color);
      }
    }
  } else if (event->action == CM_MOUSE_MOVE) {

    if (event->info.pos[1] < WINDOW_HEIGHT - top_bar_height - (quad_size / 2) &&
        cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      if (quad_count < MAX_QUADS) {
        quads[quad_count].pos[0] = event->info.pos[0] - quad_size / 2;
        quads[quad_count].pos[1] = event->info.pos[1] - quad_size / 2;
        quads[quad_count].z = layer;
        quads[quad_count].size[0] = (float)quad_size;
        quads[quad_count].size[1] = (float)quad_size;

        glm_vec4_copy(quad_color, quads[quad_count].color);

        quad_count++;
      }
    }
  }
}

ClaymoreConfig claymore_config(void) {
  return (const ClaymoreConfig){
      .window = {WINDOW_WIDTH, WINDOW_HEIGHT, .title = "Sandbox Draw"},
      .log = {
          {(cm_log_fn)fprintf, stdout},
          {(cm_log_fn)fprintf, stderr},
      }};
}

void claymore_init(CmApp *app) {
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)_sandbox_controll);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)_sandbox_mouse);

  quad_shaders.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                             "res/shader/basic.fs.glsl");
  quad_shaders.u_loc.mvp =
      cm_shader_get_uniform_location(quad_shaders.id, "u_mvp");

  glm_mat4_identity(quad_model);

  glm_ortho(0.0F, (float)WINDOW_WIDTH, 0.0F, (float)WINDOW_HEIGHT, near, 100.0F,
            app->camera.projection);

  vec3 up = {0, 1, 0};
  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (float *)up, app->camera.view);

  glClearColor(1.F, 1.F, 1.F, 1.F);

  top_bar_colors[0].size[0] = top_bar_color_quad_width;
  top_bar_colors[0].size[1] = top_bar_height;

  top_bar_colors[0].pos[0] = top_bar_color_quad_width * 0.F;
  top_bar_colors[0].pos[1] = WINDOW_HEIGHT - top_bar_height;

  top_bar_colors[0].color[0] = 1.F;
  top_bar_colors[0].color[1] = 1.F;
  top_bar_colors[0].color[2] = 1.F;
  top_bar_colors[0].color[3] = 1.F;

  top_bar_colors[1].size[0] = top_bar_color_quad_width;
  top_bar_colors[1].size[1] = top_bar_height;

  top_bar_colors[1].pos[0] = top_bar_color_quad_width * 1.F;
  top_bar_colors[1].pos[1] = WINDOW_HEIGHT - top_bar_height;

  top_bar_colors[1].color[0] = 0.F;
  top_bar_colors[1].color[1] = 0.F;
  top_bar_colors[1].color[2] = 0.F;
  top_bar_colors[1].color[3] = 1.F;

  for (size_t i = 2; i < TOP_BAR_COLOR_COUNT; ++i) {
    top_bar_colors[i].size[0] = top_bar_color_quad_width;
    top_bar_colors[i].size[1] = top_bar_height;

    top_bar_colors[i].pos[0] = top_bar_color_quad_width * i;
    top_bar_colors[i].pos[1] = WINDOW_HEIGHT - top_bar_height;

    top_bar_colors[i].color[0] = (double)rand() / (double)RAND_MAX;
    top_bar_colors[i].color[1] = (double)rand() / (double)RAND_MAX;
    top_bar_colors[i].color[2] = (double)rand() / (double)RAND_MAX;
    top_bar_colors[i].color[3] = 1.F;
  }
}

void claymore_update(CmApp *app) {
  mat4 vp;
  mat4 mvp;
  // Calculates camera perspective
  glm_mat4_mul(app->camera.projection, app->camera.view, vp);

  glm_mat4_mul(vp, quad_model, mvp);
  glUseProgram(quad_shaders.id);
  glUniformMatrix4fv(quad_shaders.u_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer_begin();
  {
    Quad2D quad;
    vec2 mouse_pos;
    cm_mouseinfo_pos(mouse_pos);
    quad.pos[0] = mouse_pos[0] - quad_size / 2;
    quad.pos[1] = mouse_pos[1] - quad_size / 2;
    quad.z = layer + layer_inc;
    quad.size[0] = (float)quad_size;
    quad.size[1] = (float)quad_size;

    glm_vec4_copy(quad_color, quad.color);

    cm_renderer_push_quad_color(quad.pos, quad.z, quad.size, quad.color);
  }
  cm_renderer_end();

  cm_renderer_begin();
  {
    for (uint32_t i = 0; i < TOP_BAR_COLOR_COUNT; ++i) {
      Quad2D *quad = &top_bar_colors[i];
      cm_renderer_push_quad_color(quad->pos, quad->z, quad->size, quad->color);
    }
  }
  cm_renderer_end();

  cm_renderer_begin();
  {
    for (uint32_t i = 0; i < quad_count; ++i) {
      Quad2D *quad = &quads[i];
      cm_renderer_push_quad_color(quad->pos, quad->z, quad->size, quad->color);
    }
  }
  cm_renderer_end();
}

void claymore_free(CmApp *app) { (void)app; }
