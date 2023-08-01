
#include "claymore.h"
#include <stdio.h>

static const float near = 0.1F;
static vec3 up = {0, 1, 0};

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct {
  GLuint vbo;
  GLuint vba;
  GLuint ibo;
} RenderData;
static struct ShaderData shader;

float rotation = 1.F;
mat4 model;

static void sandbox_mouse_callback(CmApp *app, CmMouseEvent *event) {
  (void)app;
  if (event->action == CM_MOUSE_CLICK) {
    printf("LAYER CLICK!\n");
    event->base.handled = true;
  }
}

static void sandbox_key_callback(CmApp *app, CmKeyEvent *event) {
  (void)app;
  if (event->action == CM_KEY_PRESS) {
    if (event->code == CM_KEY_F5) {
      glm_mat4_identity(model);
    }
  }
}

static bool sandbox_init(CmLayerData *layer) {
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)sandbox_key_callback);
  cm_event_set_callback(CM_EVENT_MOUSE,
                        (cm_event_callback)sandbox_mouse_callback);
  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.uniform_loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

  const float fov = 45.F;
  glm_perspective(glm_rad(fov),
                  (float)layer->app->window->width /
                      (float)layer->app->window->height,
                  near, 100.F, layer->camera.projection);

  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (float *)up, layer->camera.view);

  const struct Vertex {
    vec3 pos;
    vec4 color;
  } vertecies[] = {
      {{-0.5F, -0.5F, 0.F}, {1.F, 0.F, 0.F, 1.F}},
      {{0.5F, -0.5F, 0.F}, {0.F, 1.F, 0.F, 1.F}},
      {{0.0F, 0.5F, 0.F}, {0.F, 0.F, 1.F, 1.F}},
  };

  glGenBuffers(1, &RenderData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, RenderData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

  glGenVertexArrays(1, &RenderData.vba);
  glBindVertexArray(RenderData.vba);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)offsetof(struct Vertex, pos));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)offsetof(struct Vertex, color)); // NOLINT

  uint32_t indices[3] = {0, 2, 1};
  glGenBuffers(1, &RenderData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.ibo);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glm_mat4_identity(model);
  return true;
}
static bool sandbox_update(CmLayerData *layer) {
  mat4 vp;
  mat4 mvp;

  if (cm_key(CM_KEY_LEFT)) {
    glm_rotate(model, glm_rad(rotation), (vec3){0.F, 1.F, 0.F});
  }
  if (cm_key(CM_KEY_RIGHT)) {
    glm_rotate(model, glm_rad(rotation), (vec3){0.F, -1.F, 0.F});
  }
  if (cm_key(CM_KEY_UP)) {
    glm_rotate(model, glm_rad(rotation), (vec3){1.F, 0.F, 0.F});
  }
  if (cm_key(CM_KEY_DOWN)) {
    glm_rotate(model, glm_rad(rotation), (vec3){-1.F, 0.F, 0.F});
  }

  // Calculates camera perspective
  glm_mat4_mul(layer->camera.projection, layer->camera.view, vp);

  glm_mat4_mul(vp, model, mvp);
  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.ibo);
  glBindVertexArray(RenderData.vba);

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

  return true;
}

static bool sandbox_free(CmLayerData *layer) {
  (void)layer;
  return true;
}

CmLayer sandbox_layer(void) {
  return (CmLayer){
      .init = sandbox_init, .free = sandbox_free, .update = sandbox_update};
}
