#include "sandbox.h"
#include "claymore/core/app.h"
#include "claymore/events/event.h"
#include "claymore/events/key.h"
#include "claymore/events/mouse.h"
#include "claymore/logger/logger.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "cglm/cglm.h"
#include <GL/gl.h>
#include <time.h>

const int WINDOW_WIDTH = 620;
const int WINDOW_HEIGHT = 420;
const float near = -0.1F;

struct ShaderData {
  uint32_t id;

  struct {
    GLint time;
    GLint mvp;
    GLint color;
    GLint mouse_pos;
    GLint distance;
  } u_loc;
};

static struct ShaderData overlay_shader;
static struct ShaderData quads_shader;

mat4 overlay_model;
mat4 quads_model;
const vec4 puregym_colors = {0.F, 0.6F, 0.615F, 1.F};

typedef struct {
  float size;
  vec2 pos;
  float z;
  vec4 color;
} Quad2D;

static Quad2D quad;

const float initial_distance = 100.F;
static float distance = initial_distance;

static void _sandbox_controll(CmKeyEvent *event) {

  if (event->action == CM_KEY_PRESS && event->code == CM_KEY_W) {
    cm_log_dbg("did press w!\n");
    event->base.handled = true;
  }

  if (event->action == CM_KEY_PRESS && event->code == CM_KEY_F5) {
    cm_log_dbg("RESET\n");

    cm_mouseinfo_set_pos((vec2){100.F, 100.F});
    cm_event_dispatch((CmEvent){
        .type = CM_EVENT_MOUSE,
        .event.mouse.action = CM_MOUSE_MOVE,
        .event.mouse.info = cm_mouseinfo(),
    });
    distance = initial_distance;
  }
}

void _sandbox_mouse(CmMouseEvent *event) {
  (void)event;
  cm_log_dbg("left: %d\n", cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT));
  cm_log_dbg("right: %d\n", cm_mouseinfo_button(CM_MOUSE_BUTTON_RIGHT));
}

ClaymoreConfig cm_app_config(void) {
  return (const ClaymoreConfig){
      .window = {WINDOW_WIDTH, WINDOW_HEIGHT, .title = "Sandbox"},
      .log = {
          {(cm_log_fn)fprintf, stdout},
          {(cm_log_fn)fprintf, stderr},
      }};
}

void cm_app_init(ClaymoreApp *app) {
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)_sandbox_controll);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)_sandbox_mouse);

  quads_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                             "res/shader/basic.fs.glsl");

  quads_shader.u_loc.mvp =
      cm_shader_get_uniform_location(quads_shader.id, "u_mvp");
  quads_shader.u_loc.mouse_pos =
      cm_shader_get_uniform_location(quads_shader.id, "u_mouse_pos");
  quads_shader.u_loc.distance =
      cm_shader_get_uniform_location(quads_shader.id, "u_distance");

  overlay_shader.id = cm_load_shader_from_file(
      "res/shader/basic.vs.glsl", "res/shader/basic_uniform.fs.glsl");

  overlay_shader.u_loc.mvp =
      cm_shader_get_uniform_location(overlay_shader.id, "u_mvp");
  overlay_shader.u_loc.color =
      cm_shader_get_uniform_location(overlay_shader.id, "u_color");

  glm_mat4_identity(overlay_model);
  glm_mat4_identity(quads_model);

  glm_ortho(0.0F, (float)WINDOW_WIDTH, 0.0F, (float)WINDOW_HEIGHT, near, 100.0F,
            app->camera.projection);

  vec3 up = {0, 1, 0};
  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (float *)up, app->camera.view);
}

void cm_app_update(ClaymoreApp *app) {
  mat4 vp;
  mat4 mvp;
  // Calculates camera perspective
  glm_mat4_mul(app->camera.projection, app->camera.view, vp);

  if (cm_key(CM_KEY_W) == CM_KEY_PRESS || cm_key(CM_KEY_W) == CM_KEY_REPEAT) {
    distance += 1.F;
  }
  if (cm_key(CM_KEY_S) == CM_KEY_PRESS || cm_key(CM_KEY_S) == CM_KEY_REPEAT) {
    distance -= 1.F;
  }

  cm_renderer_begin();
  {
    glm_mat4_mul(vp, quads_model, mvp);
    glUseProgram(quads_shader.id);

    vec2 mouse_pos;
    cm_mouseinfo_pos(mouse_pos);
    glUniform2fv(quads_shader.u_loc.mouse_pos, 1, mouse_pos);
    glUniform1f(quads_shader.u_loc.distance, distance);

    glUniformMatrix4fv(quads_shader.u_loc.mvp, 1, GL_FALSE, (float *)mvp);

    const size_t grid_size = 15;
    const float xs = (WINDOW_WIDTH / (float)grid_size);
    const float ys = (WINDOW_HEIGHT / (float)grid_size);
    for (size_t i = 0; i < grid_size; ++i) {
      for (size_t j = 0; j < grid_size; ++j) {

        quad.pos[0] = xs * i;
        quad.pos[1] = ys * j;
        quad.z = 0.F;

        quad.color[0] = (i / (float)grid_size);
        quad.color[1] = (j / (float)grid_size);
        quad.color[2] = 1.F - ((j + i) / (float)(grid_size + grid_size));
        quad.color[3] = 1.F;
        cm_renderer_push_quad_color(quad.pos, quad.z, (vec2){xs, ys},
                                    quad.color);
      }
    }
  }
  cm_renderer_end();

  cm_renderer_begin();
  {
    glm_mat4_mul(vp, overlay_model, mvp);

    glUseProgram(overlay_shader.id);
    glUniformMatrix4fv(overlay_shader.u_loc.mvp, 1, GL_FALSE, (float *)mvp);
    glUniform4f(overlay_shader.u_loc.color, puregym_colors[0],
                puregym_colors[1], puregym_colors[2], puregym_colors[3]);
    cm_renderer_push_quad((vec2){0.F, 0.F}, -1.F,
                          (vec2){(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT});
  }
  cm_renderer_end();
}

void cm_app_free(void) {}
