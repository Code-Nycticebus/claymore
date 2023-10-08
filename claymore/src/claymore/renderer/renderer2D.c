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

struct Render2dData {
  CmRenderBuffer renderer;

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
  render_data->renderer.vertex_buffer = cm_vertex_buffer_create(
      CM_RENDERER2D_MAX_VERTICES, sizeof(CmVertex2D), NULL, GL_DYNAMIC_DRAW);

  render_data->vertices_count = 0;

  render_data->renderer.vertex_attribute =
      cm_vertex_attribute_create(&render_data->renderer.vertex_buffer);
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(CmVertex2D, pos));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 4, GL_FLOAT,
                           offsetof(CmVertex2D, color));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 2, GL_FLOAT,
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

  render_data->renderer.index_buffer = cm_index_buffer_create(
      &render_data->renderer.vertex_attribute, CM_RENDERER2D_MAX_INDICES,
      render_data->indices, GL_STATIC_DRAW);

  const float bytes_in_kilboytes = 1000.F;
  cm_log_info("Renderer 2D: %.0f kb\n",
              (uint32_t)sizeof(struct Render2dData) / bytes_in_kilboytes);
}

void cm_renderer2d_shutdown(void) {
  glDeleteBuffers(1, &render_data->renderer.vertex_buffer.id);
  glDeleteVertexArrays(1, &render_data->renderer.vertex_attribute.id);
  glDeleteBuffers(1, &render_data->renderer.index_buffer.id);
  free(render_data);
}

static void cm_renderer2d_flush(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->renderer.vertex_buffer.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(CmVertex2D) * render_data->vertices_count,
                  render_data->data);

  cm_renderer_draw_indexed(&render_data->renderer, render_data->indices_count);

  render_data->vertices_count = 0;
  render_data->indices_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cm_renderer2d_begin(void) {}

void cm_renderer2d_end(void) { cm_renderer2d_flush(); }

static inline void _cm_renderer2d_push_quad(const vec2s position, const float z,
                                            const vec2s size, const vec4s color,
                                            const vec2s text_coord,
                                            const vec2s text_size,
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
      {{position.x, position.y + size.y, z}},
      {{position.x + size.x, position.y + size.y, z}},
      {{position.x + size.x, position.y, z}},
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
    vertices[i].uv.u = text_coord.u + (i == 1 || i == 2 ? text_size.x : 0);
    vertices[i].uv.v = text_coord.v + (i == 2 || i == 3 ? text_size.y : 0);

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
                           (vec2s){0}, (vec2s){0}, 0.F);
}

void cm_renderer2d_push_quad_rotated(const vec2s position, const float z,
                                     const vec2s size, const float rotation) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F},
                           (vec2s){0}, (vec2s){0}, rotation);
}

void cm_renderer2d_push_quad_textured(const vec2s position, const float z,
                                      const vec2s size,
                                      const vec2s texture_coord,
                                      const vec2s texture_size) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F},
                           texture_coord, texture_size, 0.F);
}

void cm_renderer2d_push_quad_textured_rotated(const vec2s position,
                                              const float z, const vec2s size,
                                              const vec2s texture_coord,
                                              const vec2s texture_size,
                                              const float rotation) {
  _cm_renderer2d_push_quad(position, z, size,
                           (vec4s){.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F},
                           texture_coord, texture_size, rotation);
}

void cm_renderer2d_push_quad_color(const vec2s position, const float z,
                                   const vec2s size, const vec4s color) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2s){0}, (vec2s){0}, 0);
}

void cm_renderer2d_push_quad_color_rotated(const vec2s position, const float z,
                                           const vec2s size, const vec4s color,
                                           const float rotation) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2s){0}, (vec2s){0},
                           rotation);
}
