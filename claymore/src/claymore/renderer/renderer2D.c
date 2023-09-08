#include <assert.h>
#include <stddef.h>
#define _CM_RENDERER_INTERNAL
#include "renderer2D.h"

#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

#include "claymore/debug/debug.h"
#include "render_buffer.h"
#include "render_command.h"

struct Render2dData {
  CmRenderBuffer renderer;

  size_t vertices_count;
  CmVertex data[CM_RENDERER2D_MAX_VERTICES];

  size_t indecies_count;
  uint32_t indecies[CM_RENDERER2D_MAX_INDECIES];
};

static struct Render2dData *render_data = NULL;

void cm_renderer2d_init(void) {
  // INITS VERTEX ARRAY
  assert(render_data == NULL && "Renderer was initialized twice!");
  render_data = (struct Render2dData *)calloc(1, sizeof(struct Render2dData));

  // Initialize Quad primitive buffer
  render_data->renderer.vertex_buffer =
      cm_vertex_buffer_create(CM_RENDERER2D_MAX_VERTICES, sizeof(CmVertex),
                              render_data->data, GL_DYNAMIC_DRAW);

  render_data->vertices_count = 0;

  render_data->renderer.vertex_attribute =
      cm_vertex_attribute_create(&render_data->renderer.vertex_buffer);
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(CmVertex, pos));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 4, GL_FLOAT,
                           offsetof(CmVertex, color));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 2, GL_FLOAT,
                           offsetof(CmVertex, uv));

  // Generates indices in advance
  uint32_t base_indices[CM_RENDERER_INDICES_PER_SQUAD] = {
      0, 1, 3, 1, 2, 3,
  };
  for (size_t i = 0; i < CM_RENDERER2D_MAX_INDECIES; ++i) {
    render_data->indecies[i] = base_indices[i % CM_RENDERER_INDICES_PER_SQUAD] +
                               (4 * (i / CM_RENDERER_INDICES_PER_SQUAD));
  }
  render_data->indecies_count = 0;

  render_data->renderer.index_buffer = cm_index_buffer_create(
      &render_data->renderer.vertex_attribute, CM_RENDERER2D_MAX_INDECIES,
      render_data->indecies, GL_STATIC_DRAW);
}

void cm_renderer2d_shutdown(void) {
  glDeleteBuffers(1, &render_data->renderer.vertex_buffer.id);
  glDeleteVertexArrays(1, &render_data->renderer.vertex_attribute.id);
  glDeleteBuffers(1, &render_data->renderer.index_buffer.id);
  free(render_data);
}

void cm_renderer2d_begin(void) {}

void cm_renderer2d_end(void) { cm_renderer2d_flush(); }

void cm_renderer2d_flush(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->renderer.vertex_buffer.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(CmVertex) * render_data->vertices_count,
                  render_data->data);

  cm_renderer_draw_indexed(&render_data->renderer, render_data->indecies_count);

  render_data->vertices_count = 0;
  render_data->indecies_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static inline void _cm_renderer2d_push_quad(const vec2 position, float z,
                                            const vec2 size, const vec4 color,
                                            const vec2 text_coord,
                                            const vec2 text_size,
                                            float rotation) {
  if (!(render_data->vertices_count < CM_RENDERER2D_MAX_VERTICES)) {
    cm_renderer2d_flush();
  }

  // For safety i still assert
  assert(render_data->vertices_count < CM_RENDERER2D_MAX_VERTICES);
  assert(render_data->indecies_count < CM_RENDERER2D_MAX_INDECIES);

  const vec2 vertex_positions[CM_RENDERER2D_VERTICES_PER_QUAD] = {
      {position[0], position[1]},
      {position[0] + size[0], position[1]},
      {position[0] + size[0], position[1] + size[1]},
      {position[0], position[1] + size[1]},
  };

  float cos_theta;
  float sin_theta;
  if (rotation != 0) {
    cos_theta = cosf(rotation);
    sin_theta = sinf(rotation);
  }

  CmVertex *vertices = &render_data->data[render_data->vertices_count];
  for (int i = 0; i < CM_RENDERER2D_VERTICES_PER_QUAD; ++i) {
    vertices[i].pos[0] = vertex_positions[i][0];
    vertices[i].pos[1] = vertex_positions[i][1];
    vertices[i].pos[2] = z;
    vertices[i].color[0] = color[0];
    vertices[i].color[1] = color[1];
    vertices[i].color[2] = color[2];
    vertices[i].color[3] = color[3];
    vertices[i].uv[0] = text_coord[0] + (i == 1 || i == 2 ? text_size[0] : 0);
    vertices[i].uv[1] = text_coord[1] + (i == 2 || i == 3 ? text_size[1] : 0);

    if (rotation != 0.F) {
      float x = vertices[i].pos[0] - position[0];
      float y = vertices[i].pos[1] - position[1];
      vertices[i].pos[0] = x * cos_theta - y * sin_theta + position[0];
      vertices[i].pos[1] = x * sin_theta + y * cos_theta + position[1];
    }
  }

  render_data->vertices_count += CM_RENDERER2D_VERTICES_PER_QUAD;
  render_data->indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}

void cm_renderer2d_push_quad(const vec2 position, float z, const vec2 size) {
  _cm_renderer2d_push_quad(position, z, size, (vec4){1.F, 1.F, 1.F, 1.F},
                           (vec2){0}, (vec2){0}, 0.F);
}

void cm_renderer2d_push_quad_rotated(const vec2 position, float z,
                                     const vec2 size, float rotation) {
  _cm_renderer2d_push_quad(position, z, size, (vec4){1.F, 1.F, 1.F, 1.F},
                           (vec2){0}, (vec2){0}, rotation);
}

void cm_renderer2d_push_quad_textured(const vec2 position, float z,
                                      const vec2 size, const vec2 texture_coord,
                                      const vec2 texture_size) {
  _cm_renderer2d_push_quad(position, z, size, (vec4){1.F, 1.F, 1.F, 1.F},
                           texture_coord, texture_size, 0.F);
}

void cm_renderer2d_push_quad_textured_rotated(const vec2 position, float z,
                                              const vec2 size,
                                              const vec2 texture_coord,
                                              const vec2 texture_size,
                                              float rotation) {
  _cm_renderer2d_push_quad(position, z, size, (vec4){1.F, 1.F, 1.F, 1.F},
                           texture_coord, texture_size, rotation);
}

void cm_renderer2d_push_quad_color(const vec2 position, float z,
                                   const vec2 size, const vec4 color) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2){0}, (vec2){0}, 0.F);
}

void cm_renderer2d_push_quad_color_rotated(const vec2 position, float z,
                                           const vec2 size, const vec4 color,
                                           float rotation) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2){0}, (vec2){0},
                           rotation);
}
