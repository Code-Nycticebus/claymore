#include "quads.h"

#include "claymore/renderer/gpu.h"
#include "claymore/renderer/shaders.h"

#include <stdlib.h>

#define CM_QUADS_MAX 1000

#define CM_QUADS_VERTICES 4
#define CM_QUADS_VERTICES_MAX (CM_QUADS_MAX * CM_QUADS_VERTICES)
#define CM_QUADS_INDICES 6
#define CM_QUADS_INDICES_MAX (CM_QUADS_MAX * CM_QUADS_INDICES)

typedef struct {
  vec2 pos;
  vec4 color;
} Vertex;

struct RenderQuadData {
  Arena arena;
  CmGpu gpu;

  CmShader shader;

  CmVbo vbo;
  CmVao vao;
  CmEbo ebo;

  size_t vertices_count;
  Vertex data[CM_QUADS_VERTICES_MAX];

  size_t indices_count;
  uint32_t indices[CM_QUADS_INDICES_MAX];
};

struct RenderQuadData *renderer = NULL;

static void cm_quad_flush(void) {
  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertices_count,
                    (float *)renderer->data);

  cm_gpu_vao_bind(&renderer->vao);

  cm_gpu_ebo_draw(&renderer->ebo, renderer->indices_count, CM_DRAW_TRIANGLES);

  renderer->indices_count = 0;
  renderer->vertices_count = 0;
}

void cm_quad_begin(const mat4 mvp) {
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"), mvp);
}
void cm_quad_end(void) { cm_quad_flush(); }

void cm_quad_push(const vec2 position, const vec2 size, float rotation,
                  const vec4 color) {
  clib_assert_debug(renderer, "Renderer 2D was not initialized!");
  if (!(renderer->vertices_count < CM_QUADS_VERTICES_MAX)) {
    cm_quad_flush();
  }

  // For safety i still assert
  clib_assert_debug(renderer->vertices_count < CM_QUADS_VERTICES_MAX, "");
  clib_assert_debug(renderer->indices_count < CM_QUADS_INDICES_MAX, "");

  float cos_theta;
  float sin_theta;
  if (rotation != 0) {
    cos_theta = cosf(rotation);
    sin_theta = sinf(rotation);
  }

  const vec2 quad[CM_QUADS_VERTICES] = {
      {position[0], position[1]},
      {position[0] + size[0], position[1]},
      {position[0] + size[0], position[1] + size[1]},
      {position[0], position[1] + size[1]},
  };
  Vertex *vertices = &renderer->data[renderer->vertices_count];
  for (int i = 0; i < CM_QUADS_VERTICES; ++i) {
    vertices[i].pos[0] = quad[i][0];
    vertices[i].pos[1] = quad[i][1];

    if (rotation != 0.f) {
      float x = vertices[i].pos[0] - position[0];
      float y = vertices[i].pos[1] - position[1];
      vertices[i].pos[0] = x * cos_theta - y * sin_theta + position[0];
      vertices[i].pos[1] = x * sin_theta + y * cos_theta + position[1];
    }

    vertices[i].color[0] = color[0];
    vertices[i].color[1] = color[1];
    vertices[i].color[2] = color[2];
    vertices[i].color[3] = color[3];
  }

  renderer->vertices_count += CM_QUADS_VERTICES;
  renderer->indices_count += CM_QUADS_INDICES;
}

void cm_quad_internal_init(void) {
  renderer = calloc(1, sizeof(struct RenderQuadData));
  renderer->gpu = cm_gpu_internal_init(&renderer->arena);

  renderer->vbo = cm_gpu_vbo(&renderer->gpu, CM_DYNAMIC_DRAW, sizeof(Vertex),
                             CM_QUADS_VERTICES_MAX, (float *)&renderer->data);

  renderer->vao = cm_gpu_vao(&renderer->gpu);
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, pos));
  cm_gpu_vao_push(&renderer->vao, 4, sizeof(Vertex), offsetof(Vertex, color));

  u32 indices[CM_QUADS_INDICES] = {0, 1, 3, 1, 2, 3};
  for (size_t i = 0; i < CM_QUADS_INDICES_MAX; ++i) {
    renderer->indices[i] = indices[i % CM_QUADS_INDICES];
    renderer->indices[i] += (CM_QUADS_VERTICES * (i / CM_QUADS_INDICES));
  }
  renderer->ebo = cm_gpu_ebo(&renderer->gpu, CM_STATIC_DRAW,
                             CM_QUADS_INDICES_MAX, renderer->indices);

  renderer->shader = cm_shader_from_memory( // Format like this pls
      STR("#version 330 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "layout (location = 1) in vec4 a_color;\n"
          "uniform mat4 u_mvp;\n"
          "out vec4 v_color;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0.0, 1.0);\n"
          "  v_color = a_color;\n"
          "}\n"),
      STR("#version 330 core\n"
          "in vec4 v_color;\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  f_color = v_color;\n"
          "}\n"),
      ErrPanic);

  const usize bytes = sizeof(struct RenderQuadData) / 1000;
  clib_log_info("Renderer: %" USIZE_FMT " kb", bytes);
}

void cm_quad_internal_free(void) {
  cm_shader_delete(&renderer->shader);
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}
