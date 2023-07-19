#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

#include "cglm/affine-pre.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include <GL/gl.h>
#include <time.h>

const int WINDOW_WIDTH = 620 * 2;
const int WINDOW_HEIGHT = 420 * 2;
const float near = -0.1F;

struct ShaderData {
  uint32_t id;

  struct {
    GLint time;
    GLint mvp;
    GLint color;
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

ClaymoreConfig cm_app_config(void) {
  return (ClaymoreConfig){
      .window = {WINDOW_WIDTH, WINDOW_HEIGHT, .title = "Sandbox"}};
}

void cm_app_init(ClaymoreApp *app) {

  quads_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                             "res/shader/basic.fs.glsl");

  quads_shader.u_loc.mvp = glGetUniformLocation(quads_shader.id, "u_mvp");
  if (quads_shader.u_loc.mvp == -1) {
    fprintf(stderr, "Uniform location '%s' not found in shader %u\n", "u_mvp",
            quads_shader.id);
  }

  overlay_shader.id = cm_load_shader_from_file(
      "res/shader/basic.vs.glsl", "res/shader/basic_uniform.fs.glsl");
  overlay_shader.u_loc.mvp = glGetUniformLocation(overlay_shader.id, "u_mvp");
  if (overlay_shader.u_loc.mvp == -1) {
    fprintf(stderr, "Uniform location '%s' not found in shader %u\n", "u_mvp",
            overlay_shader.id);
  }
  overlay_shader.u_loc.color =
      glGetUniformLocation(overlay_shader.id, "u_color");
  if (overlay_shader.u_loc.color == -1) {
    fprintf(stderr, "Uniform location '%s' not found in shader %u\n", "u_color",
            overlay_shader.id);
  }

  glm_mat4_identity(overlay_model);
  glm_mat4_identity(quads_model);

  glm_ortho(0.0F, WINDOW_WIDTH, 0.0F, WINDOW_HEIGHT, near, 100.0F,
            app->camera.projection);

  vec3 up = {0, 1, 0};
  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (float *)up, app->camera.view);
}

void cm_app_update(ClaymoreApp *app) {
  mat4 vp;
  mat4 mvp;
  // Calculates camera perspective
  glm_mat4_mul(app->camera.projection, app->camera.view, vp);

  cm_renderer_begin();
  {
    glm_mat4_mul(vp, quads_model, mvp);
    glUseProgram(quads_shader.id);
    glUniformMatrix4fv(quads_shader.u_loc.mvp, 1, GL_FALSE, (float *)mvp);

    const size_t grid_size = 30;
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
                          (vec2){WINDOW_WIDTH, WINDOW_HEIGHT});
  }
  cm_renderer_end();
}

void cm_app_free(void) {}
