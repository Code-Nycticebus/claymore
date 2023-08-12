#include "claymore.h"

#include "claymore/renderer/render_buffer.h"

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

static CmRenderBuffer render_data;

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
  const size_t vertecies_count = 3;

  render_data.vertex_buffer = cm_vertex_buffer_create(
      vertecies_count, sizeof(struct Vertex), vertecies, GL_STATIC_DRAW);

  render_data.vertex_attribute =
      cm_vertex_attribute_create(&render_data.vertex_buffer);
  cm_vertex_attribute_push(&render_data.vertex_attribute, 3, GL_FLOAT,
                           offsetof(struct Vertex, pos));
  cm_vertex_attribute_push(&render_data.vertex_attribute, 4, GL_FLOAT,
                           offsetof(struct Vertex, color));

  uint32_t indices[] = {0, 1, 2};
  const uint32_t indices_count = 3;
  render_data.index_buffer = cm_index_buffer_create(
      &render_data.vertex_attribute, indices_count, indices, GL_STATIC_DRAW);
}

static void ortho_update(CmLayer *layer, float dt) {
  (void)dt;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(ortho_shader.id);
  glUniformMatrix4fv(ortho_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer_draw_indexed(&render_data, render_data.index_buffer.count);

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
