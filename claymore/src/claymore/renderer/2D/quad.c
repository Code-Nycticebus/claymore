#include "renderer2D.h"

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

struct QuadRenderer {
  Arena arena;
  CmGpu gpu;

  CmShader shader;
  CmCamera2D *camera;

  CmVbo vbo;
  CmVao vao;
  CmEbo ebo;

  size_t vertices_count;
  Vertex data[CM_QUADS_VERTICES_MAX];

  size_t indices_count;
  uint32_t indices[CM_QUADS_INDICES_MAX];
};

static struct QuadRenderer *renderer = NULL;

static void cm_quad_flush(void) {
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"),
                     renderer->camera->base.vp);

  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertices_count,
                    (float *)renderer->data);

  cm_gpu_vao_bind(&renderer->vao);
  glDisable(GL_DEPTH_TEST);
  cm_gpu_ebo_draw(&renderer->ebo, renderer->indices_count, CM_DRAW_TRIANGLES);
  glEnable(GL_DEPTH_TEST);

  renderer->indices_count = 0;
  renderer->vertices_count = 0;
}

void cm_quad(const vec2 pos, const vec2 s, float r, const vec4 color) {
  cebus_assert_debug(renderer, "Renderer 2D was not initialized!");
  if (!(renderer->vertices_count < CM_QUADS_VERTICES_MAX)) {
    cm_quad_flush();
  }

  // For safety i still assert
  cebus_assert_debug(renderer->vertices_count < CM_QUADS_VERTICES_MAX, "");
  cebus_assert_debug(renderer->indices_count < CM_QUADS_INDICES_MAX, "");

  float cos_theta = 1;
  float sin_theta = 0;
  if (r != 0) {
    cos_theta = cosf(r);
    sin_theta = sinf(r);
  }

  vec2 quad[CM_QUADS_VERTICES] = {
      {0, 0},
      {s[0], 0},
      {s[0], s[1]},
      {0, s[1]},
  };
  Vertex *vertices = &renderer->data[renderer->vertices_count];
  for (int i = 0; i < CM_QUADS_VERTICES; ++i) {
    if (r != 0) {
      const float x = quad[i][0];
      const float y = quad[i][1];
      quad[i][0] = x * cos_theta - y * sin_theta;
      quad[i][1] = x * sin_theta + y * cos_theta;
    }

    vertices[i].pos[0] = quad[i][0] + pos[0];
    vertices[i].pos[1] = quad[i][1] + pos[1];

    vertices[i].color[0] = color[0];
    vertices[i].color[1] = color[1];
    vertices[i].color[2] = color[2];
    vertices[i].color[3] = color[3];
  }

  renderer->vertices_count += CM_QUADS_VERTICES;
  renderer->indices_count += CM_QUADS_INDICES;
}

void cm_quad_internal_begin(CmCamera2D *camera) { renderer->camera = camera; }
void cm_quad_internal_end(void) {
  if (renderer->vertices_count) {
    cm_quad_flush();
  }
}

void *cm_quad_internal_init(void) {
  renderer = calloc(1, sizeof(struct QuadRenderer));
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

  renderer->shader = cm_shader_from_memory(
      &renderer->gpu,
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

  return renderer;
}

void cm_quad_internal_free(void) {
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}

void cm_quad_internal_use(void *r) { renderer = r; }
