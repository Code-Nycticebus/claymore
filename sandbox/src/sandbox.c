#include "sandbox.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "GL/gl.h"
#include "cglm/cglm.h"
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

#define MAX_QUADS 1000
static const uint32_t quad_size = 30;
static uint32_t quad_count;
Quad2D quads[MAX_QUADS];

static void _sandbox_controll(CmApp *app, CmKeyEvent *event) {
  (void)app;
  if (event->action == CM_KEY_PRESS && event->code == CM_KEY_F5) {
    cm_log_dbg("Press <F5>\n");
    event->base.handled = true;
  }
}

static bool _sandbox_point_in_quad(const vec2 point, const Quad2D *quad) {
  return ((point[0] > quad->pos[0]) &&
          (point[0] < quad->pos[0] + quad->size[0])) // if in x axis
         &&                                          // and
         ((point[1] > quad->pos[1]) &&               // if in y axis
          (point[1] < quad->pos[1] + quad->size[1]));
}

static bool _sandbox_point_in_quad_array(const vec2 point, uint64_t count,
                                         const Quad2D *quad_array) {
  for (uint32_t i = 0; i < count; ++i) {
    if (_sandbox_point_in_quad(point, &quad_array[i])) {
      return true;
    }
  }
  return false;
}

void _sandbox_mouse(CmApp *app, CmMouseEvent *event) { (void)event, (void)app; }

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
}

void claymore_update(CmApp *app) {
  mat4 vp;
  mat4 mvp;
  // Calculates camera perspective
  glm_mat4_mul(app->camera.projection, app->camera.view, vp);

  if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
    vec2 mouse_pos;
    cm_mouseinfo_pos(mouse_pos);

    if (!_sandbox_point_in_quad_array(mouse_pos, quad_count, quads)) {
      if (quad_count < MAX_QUADS) {
        cm_log_dbg("DRAW\n");

        quads[quad_count].pos[0] = mouse_pos[0] - quad_size / 2;
        quads[quad_count].pos[1] = mouse_pos[1] - quad_size / 2;
        quads[quad_count].size[0] = (float)quad_size;
        quads[quad_count].size[1] = (float)quad_size;

        quads[quad_count].color[0] = 1.0F;
        quads[quad_count].color[1] = 1.0F;
        quads[quad_count].color[2] = 1.0F;
        quads[quad_count].color[3] = 1.0F;

        quad_count++;
      }
    }
  }

  cm_renderer_begin();
  {
    glm_mat4_mul(vp, quad_model, mvp);
    glUseProgram(quad_shaders.id);
    glUniformMatrix4fv(quad_shaders.u_loc.mvp, 1, GL_FALSE, (float *)mvp);

    for (uint32_t i = 0; i < quad_count; ++i) {
      Quad2D *quad = &quads[i];
      cm_renderer_push_quad_color(quad->pos, quad->z, quad->size, quad->color);
    }
  }
  cm_renderer_end();
}

void claymore_free(CmApp *app) { (void)app; }
