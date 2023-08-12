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

  size_t vertecies_count;
  VertexColor2D data[CM_RENDERER2D_MAX_VERTECIES];

  size_t indecies_count;
  uint32_t indecies[CM_RENDERER2D_MAX_INDECIES];
};

static struct Render2dData *render_data = NULL;

void cm_renderer_init(void) {
  // INITS VERTEX ARRAY
  assert(render_data == NULL && "Renderer was initialized twice!");
  render_data = (struct Render2dData *)calloc(1, sizeof(struct Render2dData));

  // Initialize Quad primitive buffer
  render_data->renderer.vertex_buffer = cm_vertex_buffer_create(
      CM_RENDERER2D_MAX_VERTECIES, sizeof(VertexColor2D), render_data->data,
      GL_DYNAMIC_DRAW);

  render_data->vertecies_count = 0;

  render_data->renderer.vertex_attribute =
      cm_vertex_attribute_create(&render_data->renderer.vertex_buffer);
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(VertexColor2D, pos));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 4, GL_FLOAT,
                           offsetof(VertexColor2D, color));

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

void cm_renderer_shutdown(void) {
  glDeleteBuffers(1, &render_data->renderer.vertex_buffer.id);
  glDeleteVertexArrays(1, &render_data->renderer.vertex_attribute.id);
  glDeleteBuffers(1, &render_data->renderer.index_buffer.id);
  free(render_data);
}

void cm_renderer_begin(void) {}

void cm_renderer_end(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->renderer.vertex_buffer.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(VertexColor2D) * render_data->vertecies_count,
                  render_data->data);

  cm_renderer_draw_indexed(&render_data->renderer, render_data->indecies_count);

  render_data->vertecies_count = 0;
  render_data->indecies_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cm_renderer_push_quad(const vec2 position, float z, const vec2 size) {
  cm_renderer_push_quad_color(position, z, size, (vec4){1.F, 1.F, 1.F, 1.F});
}

void cm_renderer_push_quad_color(const vec2 position, float z, const vec2 size,
                                 const vec4 color) {
  assert(render_data->vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(render_data->indecies_count < CM_RENDERER2D_MAX_INDECIES);

  const float x = position[0];
  const float xs = size[0];
  const float y = position[1];
  const float ys = size[1];

  VertexColor2D vertecies[] = {
      {{x, y, z}, {color[0], color[1], color[2], color[3]}},           // a
      {{x + xs, y, z}, {color[0], color[1], color[2], color[3]}},      // b
      {{x + xs, y + ys, z}, {color[0], color[1], color[2], color[3]}}, // c
      {{x, y + ys, z}, {color[0], color[1], color[2], color[3]}},      // d
  };
  memcpy(&render_data->data[render_data->vertecies_count], vertecies,
         sizeof(vertecies));

  render_data->vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  render_data->indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}
