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
  CmVertex data[CM_RENDERER2D_MAX_VERTECIES];

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
      cm_vertex_buffer_create(CM_RENDERER2D_MAX_VERTECIES, sizeof(CmVertex),
                              render_data->data, GL_DYNAMIC_DRAW);

  render_data->vertecies_count = 0;

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
                  sizeof(CmVertex) * render_data->vertecies_count,
                  render_data->data);

  cm_renderer_draw_indexed(&render_data->renderer, render_data->indecies_count);

  render_data->vertecies_count = 0;
  render_data->indecies_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void _cm_renderer2d_push_quad(const vec2 position, float z,
                                     const vec2 size, const vec4 color,
                                     vec2 text_coord, vec2 text_size) {
  if (!(render_data->vertecies_count < CM_RENDERER2D_MAX_VERTECIES)) {
    cm_renderer2d_flush();
  }

  // For safety i still assert
  assert(render_data->vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(render_data->indecies_count < CM_RENDERER2D_MAX_INDECIES);

  const float x = position[0];
  const float xs = size[0];
  const float y = position[1];
  const float ys = size[1];

  CmVertex* vertecies  = &render_data->data[render_data->vertecies_count];
  vertecies[0].pos[0] = x;
  vertecies[0].pos[1] = y;
  vertecies[0].pos[2] = z;
  vertecies[0].color[0] = color[0];
  vertecies[0].color[1] = color[1];
  vertecies[0].color[2] = color[2];
  vertecies[0].color[3] = color[3];
  vertecies[0].uv[0] = text_coord[0];
  vertecies[0].uv[1] = text_coord[1];

  vertecies[1].pos[0] = x + xs;
  vertecies[1].pos[1] = y;
  vertecies[1].pos[2] = z;
  vertecies[1].color[0] = color[0];
  vertecies[1].color[1] = color[1];
  vertecies[1].color[2] = color[2];
  vertecies[1].color[3] = color[3];
  vertecies[1].uv[0] = text_coord[0] + text_size[0];
  vertecies[1].uv[1] = text_coord[1];

  vertecies[2].pos[0] = x + xs;
  vertecies[2].pos[1] = y + ys;
  vertecies[2].pos[2] = z;
  vertecies[2].color[0] = color[0];
  vertecies[2].color[1] = color[1];
  vertecies[2].color[2] = color[2];
  vertecies[2].color[3] = color[3];
  vertecies[2].uv[0] = text_coord[0] + text_size[0];
  vertecies[2].uv[1] = text_coord[1] + text_size[1];

  vertecies[3].pos[0] = x;
  vertecies[3].pos[1] = y + ys;
  vertecies[3].pos[2] = z;
  vertecies[3].color[0] = color[0];
  vertecies[3].color[1] = color[1];
  vertecies[3].color[2] = color[2];
  vertecies[3].color[3] = color[3];
  vertecies[3].uv[0] = text_coord[0];
  vertecies[3].uv[1] = text_coord[1] + text_size[1];

  render_data->vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  render_data->indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}

void cm_renderer2d_push_quad(const vec2 position, float z, const vec2 size,
                             vec2 texture_coord, vec2 texture_size) {
  _cm_renderer2d_push_quad(position, z, size, (vec4){1.F, 1.F, 1.F, 1.F},
                           texture_coord, texture_size);
}

void cm_renderer2d_push_quad_color(const vec2 position, float z,
                                   const vec2 size, const vec4 color) {
  _cm_renderer2d_push_quad(position, z, size, color, (vec2){0}, (vec2){0});
}
