#include "claymore.h"

static struct {
  GLuint vbo;
  GLuint vao;
  GLuint ibo;
  GLuint i;
} OrthoRenderData;

struct Vertex {
  vec3 pos;
  vec4 color;
};

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct ShaderData ortho_shader;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

typedef struct {
  size_t stride;
  size_t index;
} VertexAttribute;

static void cm_vertex_attribute_push(VertexAttribute *attr, GLenum type,
                                     size_t count, size_t offset) {
  glEnableVertexAttribArray(attr->index);
  glVertexAttribPointer(attr->index, count, type, GL_FALSE, attr->stride,
                        (const void *)offset); // NOLINT
  attr->index++;
}

static void ortho_init(CmLayer *layer) {
  ortho_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                             "res/shader/basic.fs.glsl");
  ortho_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(ortho_shader.id, "u_mvp");

  glm_ortho(-1.F, 1.F, -1.F, 1.F, -100.F, 100.F, layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  layer->camera.update = true;

  struct Vertex vertecies[] = {
      {{-1.F, -1.F, 0.F}, {1.F, 0.F, 0.F, 1.F}}, //
      {{1.F, -1.F, 0.F}, {0.F, 1.F, 0.F, 1.F}},  //
      {{0.F, 1.F, 0.F}, {0.F, 0.F, 1.F, 1.F}},
  };

  glGenBuffers(1, &OrthoRenderData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, OrthoRenderData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

  glGenVertexArrays(1, &OrthoRenderData.vao);
  glBindVertexArray(OrthoRenderData.vao);

  VertexAttribute attr = {.index = 0, .stride = sizeof(struct Vertex)};
  cm_vertex_attribute_push(&attr, GL_FLOAT, 3, offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&attr, GL_FLOAT, 4, offsetof(struct Vertex, color));

  GLuint indices[] = {0, 1, 2};
  OrthoRenderData.i = 3;
  glGenBuffers(1, &OrthoRenderData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OrthoRenderData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

static void ortho_update(CmLayer *layer, float dt) {
  (void)dt;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(ortho_shader.id);
  glUniformMatrix4fv(ortho_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  glBindVertexArray(OrthoRenderData.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OrthoRenderData.ibo);

  glDrawElements(GL_TRIANGLES, OrthoRenderData.i, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);
}

static void ortho_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_ortho(void) {
  return (CmLayerInterface){
      .init = ortho_init,
      .free = ortho_free,
      .update = ortho_update,
  };
}
