#define _CM_RENDERER_INTERNAL
#include "renderer2D.h"

#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

#include "claymore/debug/debug.h"

struct RenderData {
  GLuint vbo; // Vertex Buffer holds the data
  GLuint vao; // Vertex Array is there to be there???
  GLuint ibo; // Holds indecies to reduce repetition
  GLuint i_count;

  size_t vertecies_count;
  VertexColor2D data[CM_RENDERER2D_MAX_VERTECIES];

  size_t indecies_count;
  uint32_t indecies[CM_RENDERER2D_MAX_INDECIES];
};

static struct RenderData *render_data = NULL;

void cm_renderer_init(void) {
  // INITS VERTEX ARRAY
  assert(render_data == NULL && "Renderer was initialized twice!");
  render_data = (struct RenderData *)calloc(1, sizeof(struct RenderData));
  // Initialize Quad primitive buffer
  glGenBuffers(1, &render_data->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, render_data->vbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(VertexColor2D) * CM_RENDERER2D_MAX_VERTECIES, NULL,
               GL_DYNAMIC_DRAW);
  render_data->vertecies_count = 0;

  glGenVertexArrays(1, &render_data->vao);
  glBindVertexArray(render_data->vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor2D),
                        (const void *)offsetof(VertexColor2D, pos));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexColor2D),
                        // It talks about, that turning a number
                        // into a pointer, removes optimization opportunities
                        (void *)offsetof(VertexColor2D, color)); // NOLINT
  // Generates indices in advance
  uint32_t base_indices[CM_RENDERER_INDICES_PER_SQUAD] = {
      0, 1, 3, 1, 2, 3,
  };
  for (size_t i = 0; i < CM_RENDERER2D_MAX_INDECIES; ++i) {
    render_data->indecies[i] = base_indices[i % CM_RENDERER_INDICES_PER_SQUAD] +
                               (4 * (i / CM_RENDERER_INDICES_PER_SQUAD));
  }
  render_data->indecies_count = 0;

  // Loads the index buffer on the cpu
  glGenBuffers(1, &render_data->ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(render_data->indecies),
               render_data->indecies, GL_STATIC_DRAW);
}

void cm_renderer_shutdown(void) {
  glDeleteBuffers(1, &render_data->vbo);
  glDeleteVertexArrays(1, &render_data->vao);
  glDeleteBuffers(1, &render_data->ibo);
  free(render_data);
}

void cm_renderer_begin(void) {}

void cm_renderer_end(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(VertexColor2D) * render_data->vertecies_count,
                  render_data->data);
  glBindVertexArray(render_data->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data->ibo);

  cm_renderer_draw();

  render_data->vertecies_count = 0;
  render_data->indecies_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cm_renderer_draw(void) {
  glDrawElements(GL_TRIANGLES, render_data->indecies_count, GL_UNSIGNED_INT, 0);
}

void cm_renderer_push_quad(const vec2 position, float z, const vec2 size) {
  assert(render_data->vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(render_data->indecies_count < CM_RENDERER2D_MAX_INDECIES);

  const float x = position[0];
  const float x_size = size[0];
  const float y = position[1];
  const float y_size = size[1];

  VertexColor2D vertecies[] = {
      {{x, y, z}, {1.F, 1.F, 1.F, 1.F}},                   // a
      {{x + x_size, y, z}, {1.F, 1.F, 1.F, 1.F}},          // b
      {{x + x_size, y + y_size, z}, {1.F, 1.F, 1.F, 1.F}}, // c
      {{x, y + y_size, z}, {1.F, 1.F, 1.F, 1.F}},          // d
  };

  memcpy(&render_data->data[render_data->vertecies_count], vertecies,
         sizeof(vertecies));

  render_data->vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  render_data->indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
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
