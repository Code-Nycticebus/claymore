#define CM_RENDERER_PRIVATE
#include "renderer2D.h"

#include "render_buffer.h"
#include "render_command.h"

#define CM_RENDERER2D_MAX_QUADS 5000
#define CM_RENDERER2D_VERTICES_PER_QUAD 4
#define CM_RENDERER2D_MAX_VERTICES                                             \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER2D_VERTICES_PER_QUAD)
#define CM_RENDERER2D_INDICES_PER_QUAD 6
#define CM_RENDERER2D_MAX_INDICES                                              \
  (CM_RENDERER2D_MAX_QUADS * CM_RENDERER2D_INDICES_PER_QUAD)

typedef struct {
  vec3s pos;
  vec4s color;
  vec2s uv;
} CmVertex2D;

struct Render2dData {
  CmVertexBuffer vbo;
  CmVertexArray vao;
  CmIndexBuffer ibo;

  size_t vertices_count;
  CmVertex2D data[CM_RENDERER2D_MAX_VERTICES];

  size_t indices_count;
  uint32_t indices[CM_RENDERER2D_MAX_INDICES];
};

static struct Render2dData *render_data = NULL;

void cm_renderer2d_init(void) {
  // INITS VERTEX ARRAY
  assert(render_data == NULL && "Renderer2d was initialized twice!");
  render_data = (struct Render2dData *)calloc(1, sizeof(struct Render2dData));

  // Initialize Quad primitive buffer
  render_data->vbo = cm_vertex_buffer_create(
      NULL, CM_RENDERER2D_MAX_VERTICES * sizeof(CmVertex2D), CM_BUFFER_DYNAMIC);

  render_data->vertices_count = 0;

  render_data->vao = cm_vertex_array_create();
  cm_vertex_array_push_attrib(&render_data->vao, 3, sizeof(CmVertex2D),
                              offsetof(CmVertex2D, pos));
  cm_vertex_array_push_attrib(&render_data->vao, 4, sizeof(CmVertex2D),
                              offsetof(CmVertex2D, color));
  cm_vertex_array_push_attrib(&render_data->vao, 2, sizeof(CmVertex2D),
                              offsetof(CmVertex2D, uv));
  // Generates indices in advance
  uint32_t base_indices[CM_RENDERER2D_INDICES_PER_QUAD] = {
      0, 1, 3, 1, 2, 3,
  };
  for (size_t i = 0; i < CM_RENDERER2D_MAX_INDICES; ++i) {
    render_data->indices[i] = base_indices[i % CM_RENDERER2D_INDICES_PER_QUAD] +
                              (CM_RENDERER2D_VERTICES_PER_QUAD *
                               (i / CM_RENDERER2D_INDICES_PER_QUAD));
  }
  render_data->indices_count = 0;

  render_data->ibo =
      cm_index_buffer_create(render_data->indices, CM_RENDERER2D_MAX_INDICES);

  const float bytes_in_kilboytes = 1000.F;
  cm_log_info("Renderer 2D: %.0f kb\n",
              (uint32_t)sizeof(struct Render2dData) / bytes_in_kilboytes);
}

void cm_renderer2d_shutdown(void) {
  cm_vertex_buffer_delete(&render_data->vbo);
  cm_vertex_array_delete(&render_data->vao);
  cm_index_buffer_delete(&render_data->ibo);
  free(render_data);
}

static void cm_renderer2d_flush(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->vbo.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(CmVertex2D) * render_data->vertices_count,
                  render_data->data);

  glBindVertexArray(render_data->vao.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data->ibo.id);

  glDrawElements(GL_TRIANGLES, render_data->indices_count, GL_UNSIGNED_INT,
                 NULL);

  render_data->vertices_count = 0;
  render_data->indices_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void cm_renderer2d_begin(void) {}

void cm_renderer2d_end(void) { cm_renderer2d_flush(); }

static inline void _cm_renderer2d_push_quad(const vec2s position, const float z,
                                            const vec2s size, const vec4s color,
                                            const vec2s uv,
                                            const float rotation) {
  assert(render_data && "Renderer 2D was not initialized!");
  if (!(render_data->vertices_count < CM_RENDERER2D_MAX_VERTICES)) {
    cm_renderer2d_flush();
  }

  // For safety i still assert
  assert(render_data->vertices_count < CM_RENDERER2D_MAX_VERTICES);
  assert(render_data->indices_count < CM_RENDERER2D_MAX_INDICES);

  const vec3s vertex_positions[CM_RENDERER2D_VERTICES_PER_QUAD] = {
      {{position.x, position.y, z}},
      {{position.x + size.x, position.y, z}},
      {{position.x + size.x, position.y + size.y, z}},
      {{position.x, position.y + size.y, z}},
  };

  float cos_theta;
  float sin_theta;
  if (rotation != 0) {
    cos_theta = cosf(rotation);
    sin_theta = sinf(rotation);
  }

  CmVertex2D *vertices = &render_data->data[render_data->vertices_count];
  for (int i = 0; i < CM_RENDERER2D_VERTICES_PER_QUAD; ++i) {
    vertices[i].pos = vertex_positions[i];
    vertices[i].color = color;
    vertices[i].uv.u = uv.u + (i == 2 || i == 3 ? uv.v : 0);
    vertices[i].uv.v = uv.u + (i == 1 || i == 2 ? uv.v : 0);

    if (rotation != 0.F) {
      float x = vertices[i].pos.x - position.x;
      float y = vertices[i].pos.y - position.y;
      vertices[i].pos.x = x * cos_theta - y * sin_theta + position.x;
      vertices[i].pos.y = x * sin_theta + y * cos_theta + position.y;
    }
  }

  render_data->vertices_count += CM_RENDERER2D_VERTICES_PER_QUAD;
  render_data->indices_count += CM_RENDERER2D_INDICES_PER_QUAD;
}

void cm_renderer2d_push_quad(const vec2s position, const float z,
                             const vec2s size) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F},
                           (vec2s){0}, 0.F);
}

void cm_renderer2d_push_quad_rotated(const vec2s position, const float z,
                                     const vec2s size, const float rad) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F},
                           (vec2s){0}, rad);
}

void cm_renderer2d_push_quad_textured(const vec2s position, const float z,
                                      const vec2s size, const vec2s uv) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F}, uv,
                           0.F);
}

void cm_renderer2d_push_quad_textured_rotated(const vec2s position,
                                              const float z, const vec2s size,
                                              const vec2s uv, const float rad) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F}, uv,
                           rad);
}

void cm_renderer2d_push_quad_color(const vec2s position, const float z,
                                   const vec2s size, const vec4s color) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2s){0}, 0);
}

void cm_renderer2d_push_quad_color_rotated(const vec2s position, const float z,
                                           const vec2s size, const vec4s color,
                                           const float rad) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2s){0}, rad);
}
