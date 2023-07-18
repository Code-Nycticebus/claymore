#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include "cglm/cglm.h"


const int WINDOW_WIDTH = 620;
const int WINDOW_HEIGHT = 420;
const float near = -0.1F;

static struct ShaderData {
  uint32_t id;

  struct {
    GLint time;
    GLint mvp;
  } u_loc;
} shader;

static struct MVP {
  mat4 model;
  mat4 view;
  mat4 projection;
  mat4 mvp;
} mvp;

const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
    "}\n";

const char *fragment_shader_src = // what
    "#version 330 core\n"
    "out vec4 f_color;\n"
    "void main()\n"
    "{\n"
    "   f_color = vec4(0.2f, 0.615f, 0.6f, 1.0f);"
    "}\n";

typedef struct {
  float size;
  vec2 pos;
  float z;
  vec4 color;
} Quad2D;

#define MAX_QUADS 1000
static size_t quads_count = 0;
static Quad2D quad;

ClaymoreConfig cm_app_config(void) {

  return (ClaymoreConfig){
      .window = {WINDOW_WIDTH, WINDOW_HEIGHT, .title = "Sandbox"}};
}

void cm_app_init(ClaymoreApp *app) {
  (void)app;

  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");

  shader.u_loc.mvp = glGetUniformLocation(shader.id, "u_mvp");
  if (shader.u_loc.mvp == -1) {
    fprintf(stderr, "Uniform location '%s' not found in shader %u\n", "u_mvp",
            shader.id);
  }

  glm_mat4_identity(mvp.model);

  glm_ortho(0.0F, WINDOW_WIDTH, 0.0F, WINDOW_HEIGHT, near, 100.0F,
            mvp.projection);

  vec3 up = {0, 1, 0};
  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (float *)up, mvp.view);
}

void cm_app_update(ClaymoreApp *app) {
  (void)app;



  if (quads_count < MAX_QUADS) {
     
    quad.size = 100.F;
    quad.pos[0] = rand() % (int)(WINDOW_WIDTH - quad.size);
    quad.pos[1] = rand() % (int)(WINDOW_HEIGHT - quad.size);
    quad.z = 0.F;

    quad.color[0] = (double)rand() / (double)RAND_MAX;
    quad.color[1] = (double)rand() / (double)RAND_MAX;
    quad.color[2] = (double)rand() / (double)RAND_MAX;
    quad.color[3] = (double)rand() / (double)RAND_MAX;

    cm_renderer_init_quad_color(&app->renderer, quad.pos, quad.z,
                                (vec2){quad.size, quad.size}, quad.color);

    quads_count += 1;
  }

  glm_mat4_mul(mvp.projection, mvp.view, mvp.mvp);
  glm_mat4_mul(mvp.mvp, mvp.model, mvp.mvp);

  glUseProgram(shader.id);

  glUniformMatrix4fv(shader.u_loc.mvp, 1, GL_FALSE, (float *)mvp.mvp);
}

void cm_app_free(void) {}
