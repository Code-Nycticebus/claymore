
#include "renderer2D.h"

#include "claymore/assets/shaders.h"
#include "claymore/renderer/gpu.h"

#include <stdlib.h>

#define CM_LINES_MAX 1000

#define CM_LINES_VERTICES 2
#define CM_LINES_VERTICES_MAX (CM_LINES_MAX * CM_LINES_VERTICES)

typedef struct {
  vec2 pos;
} Vertex;

struct LineRenderer {
  Arena arena;
  CmGpu gpu;

  CmShader shader;
  CmCamera2D *camera;

  CmVbo vbo;
  CmVao vao;
  CmEbo ebo;

  size_t vertices_count;
  Vertex data[CM_LINES_VERTICES_MAX];
};

static struct LineRenderer *renderer = NULL;

static void cm_line_flush(void) {
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"),
                     renderer->camera->base.vp);

  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertices_count,
                    (float *)renderer->data);

  cm_gpu_vao_bind(&renderer->vao);
  cm_gpu_vbo_draw(&renderer->vbo, CM_DRAW_LINES);

  renderer->vertices_count = 0;
}

void cm_line(const vec2 from, const vec2 to) {
  cebus_assert_debug(renderer, "Renderer 2D was not initialized!");
  if (!(renderer->vertices_count < CM_LINES_VERTICES_MAX)) {
    cm_line_flush();
  }

  // For safety i still assert
  cebus_assert_debug(renderer->vertices_count < CM_LINES_VERTICES_MAX, "");

  Vertex *vertices = &renderer->data[renderer->vertices_count];

  vertices[0].pos[0] = from[0];
  vertices[0].pos[1] = from[1];

  vertices[1].pos[0] = to[0];
  vertices[1].pos[1] = to[1];

  renderer->vertices_count += CM_LINES_VERTICES;
}

void cm_line_internal_begin(CmCamera2D *camera) { renderer->camera = camera; }
void cm_line_internal_end(void) {
  if (renderer->vertices_count) {
    cm_line_flush();
  }
}

void *cm_line_internal_init(void) {
  renderer = calloc(1, sizeof(struct LineRenderer));
  renderer->gpu = cm_gpu_internal_init(&renderer->arena);

  renderer->vbo = cm_gpu_vbo(&renderer->gpu, CM_DYNAMIC_DRAW, sizeof(Vertex),
                             CM_LINES_VERTICES_MAX, (float *)&renderer->data);

  renderer->vao = cm_gpu_vao(&renderer->gpu);
  cm_gpu_vao_push(&renderer->vao, 2, sizeof(Vertex), offsetof(Vertex, pos));

  renderer->shader = cm_shader_from_memory(
      &renderer->gpu,
      STR("#version 330 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "uniform mat4 u_mvp;\n"
          "void main() {\n"
          "  gl_Position = u_mvp * vec4(a_pos.xy, 0.0, 1.0);\n"
          "}\n"),
      STR("#version 330 core\n"
          "uniform vec4 u_color = vec4(1, 1, 1, 1);\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  f_color = u_color;\n"
          "}\n"),
      ErrPanic);

  return renderer;
}

void cm_line_internal_free(void) {
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}

void cm_line_internal_use(void *r) { renderer = r; }
