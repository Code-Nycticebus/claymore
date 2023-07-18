#include "renderer2D.h"

#include <GL/gl.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void cm_renderer_init(Renderer2D *renderer) {
  // INITS VERTEX ARRAY
  glGenVertexArrays(1, &renderer->buffer.vao);
  glBindVertexArray(renderer->buffer.vao);

  // Initialize Quad primitive buffer
  glGenBuffers(1, &renderer->buffer.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->buffer.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer.data),
               renderer->buffer.data, GL_STATIC_DRAW);
  renderer->buffer.vertecies_count = 0;

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                        (const void *)offsetof(Vertex2D, pos));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                        (const void *)offsetof(Vertex2D, color));

  // Generates indices in advance
  uint32_t base_indices[CM_RENDERER_INDICES_PER_SQUAD] = {
      0, 1, 3, 1, 2, 3,
  };

  for (size_t i = 0; i < CM_RENDERER2D_MAX_INDECIES; ++i) {
    renderer->buffer.indecies[i] =
        base_indices[i % CM_RENDERER_INDICES_PER_SQUAD] +
        (4 * (i / CM_RENDERER_INDICES_PER_SQUAD));
  }
  renderer->buffer.indecies_count = 0;

  // Loads the index buffer on the cpu
  glGenBuffers(1, &renderer->buffer.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->buffer.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->buffer.indecies),
               renderer->buffer.indecies, GL_STATIC_DRAW);
}

void cm_renderer_draw(Renderer2D *renderer) {
  glBindBuffer(GL_ARRAY_BUFFER, renderer->buffer.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer.data),
               renderer->buffer.data, GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, renderer->buffer.indecies_count, GL_UNSIGNED_INT,
                 0);
}

void cm_renderer_init_quad(Renderer2D *renderer, const vec2 position, float z,
                           const vec2 size) {
  assert(renderer->buffer.vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(renderer->buffer.indecies_count < CM_RENDERER2D_MAX_INDECIES);

  const float x = position[0];
  const float x_size = size[0];
  const float y = position[1];
  const float y_size = size[1];

  Vertex2D vertecies[] = {
      {{x, y, z}, {0}},                   // a
      {{x + x_size, y, z}, {0}},          // b
      {{x + x_size, y + y_size, z}, {0}}, // c
      {{x, y + y_size, z}, {0}},          // d
  };

  memcpy(&renderer->buffer.data[renderer->buffer.vertecies_count], vertecies,
         sizeof(vertecies));

  renderer->buffer.vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  renderer->buffer.indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}

void cm_renderer_init_quad_color(Renderer2D *renderer, const vec2 position,
                                 float z, const vec2 size, vec4 color) {
  assert(renderer->buffer.vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(renderer->buffer.indecies_count < CM_RENDERER2D_MAX_INDECIES);

  const float x = position[0];
  const float x_size = size[0];
  const float y = position[1];
  const float y_size = size[1];

  Vertex2D vertecies[] = {
      {{x, y, z}, {color[0], color[1], color[2], color[3]}},          // a
      {{x + x_size, y, z}, {color[0], color[1], color[2], color[3]}}, // b
      {{x + x_size, y + y_size, z},
       {color[0], color[1], color[2], color[3]}},                     // c
      {{x, y + y_size, z}, {color[0], color[1], color[2], color[3]}}, // d
  };

  memcpy(&renderer->buffer.data[renderer->buffer.vertecies_count], vertecies,
         sizeof(vertecies));

  renderer->buffer.vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  renderer->buffer.indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}
