#include "renderer2D.h"

#include "claymore/renderer/gpu.h"
#include "claymore/renderer/shaders.h"

#define CM_CIRCLES_MAX 1000

typedef struct {
  vec2 p;
  vec2 r;
  vec4 c;
} Vertex;

struct CircleRenderer {
  Arena arena;
  CmGpu gpu;

  CmCamera2D *camera;
  CmShader shader;
  CmVbo vbo;
  CmVao vao;

  usize vertex_count;
  Vertex vertices[CM_CIRCLES_MAX];
};

struct CircleRenderer *renderer;

static void _cm_circle_flush(void) {
  cm_shader_bind(&renderer->shader);
  cm_shader_set_mat4(&renderer->shader, STR("u_mvp"),
                     cm_camera_vp(renderer->camera));

  cm_gpu_vao_bind(&renderer->vao);
  cm_gpu_vbo_update(&renderer->vbo, sizeof(Vertex), renderer->vertex_count,
                    (float *)renderer->vertices);
  cm_gpu_vbo_draw_instanced(&renderer->vbo, 4, CM_DRAW_TRIANGLE_STRIP);

  renderer->vertex_count = 0;
}

void cm_circle(const vec2 pos, const vec2 radius, const vec4 color) {
  if (!(renderer->vertex_count < CM_CIRCLES_MAX)) {
    _cm_circle_flush();
  }

  renderer->vertices[renderer->vertex_count].p[0] = pos[0];
  renderer->vertices[renderer->vertex_count].p[1] = pos[1];
  renderer->vertices[renderer->vertex_count].r[0] = radius[0];
  renderer->vertices[renderer->vertex_count].r[1] = radius[1];
  renderer->vertices[renderer->vertex_count].c[0] = color[0];
  renderer->vertices[renderer->vertex_count].c[1] = color[1];
  renderer->vertices[renderer->vertex_count].c[2] = color[2];
  renderer->vertices[renderer->vertex_count].c[3] = color[3];

  renderer->vertex_count++;
}

void cm_circle_internal_begin(CmCamera2D *camera) { renderer->camera = camera; }
void cm_circle_internal_end(void) {
  if (0 < renderer->vertex_count) {
    _cm_circle_flush();
  }
}

usize cm_circle_internal_init(void) {
  renderer = calloc(1, sizeof(*renderer));

  renderer->gpu = cm_gpu_internal_init(&renderer->arena);

  renderer->vbo =
      cm_gpu_vbo(&renderer->gpu, CM_DYNAMIC_DRAW, sizeof(renderer->vertices[0]),
                 CM_CIRCLES_MAX, NULL);

  renderer->vao = cm_gpu_vao(&renderer->gpu);
  cm_gpu_vao_instanced(&renderer->vao, 1, 2, sizeof(Vertex),
                       offsetof(Vertex, p));
  cm_gpu_vao_instanced(&renderer->vao, 1, 2, sizeof(Vertex),
                       offsetof(Vertex, r));
  cm_gpu_vao_instanced(&renderer->vao, 1, 4, sizeof(Vertex),
                       offsetof(Vertex, c));

  renderer->shader = cm_shader_from_memory(
      &renderer->gpu,
      STR("#version 430 core\n"
          "layout (location = 0) in vec2 a_pos;\n"
          "layout (location = 1) in vec2 a_radius;\n"
          "layout (location = 2) in vec4 a_color;\n"
          "uniform mat4 u_mvp;\n"
          "out vec2 v_uv;\n"
          "out vec4 v_color;\n"
          "void main() {\n"
          "  v_uv = vec2(gl_VertexID >> 0 & 1, gl_VertexID >> 1 & 1);\n"
          "  v_uv = 2 * v_uv - vec2(1.0, 1.0);\n"
          "  gl_Position = u_mvp * vec4(\n"
          "     a_pos + a_radius * v_uv,\n"
          "     0,\n"
          "     1);\n"
          "  v_color = a_color;"
          "}\n"),
      STR("#version 430 core\n"
          "in vec2 v_uv;\n"
          "in vec4 v_color;\n"
          "out vec4 f_color;\n"
          "void main() {\n"
          "  if (length(v_uv) < 1.0) {\n"
          "    f_color = v_color;\n"
          "  } else {\n"
          "    f_color = vec4(0, 0, 0, 0);\n"
          "  }\n"
          "}\n"),
      ErrPanic);

  return sizeof(*renderer);
}
void cm_circle_internal_free(void) {
  cm_gpu_internal_free(&renderer->gpu);
  arena_free(&renderer->arena);
  free(renderer);
}
