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

void cm_renderer_init_quad(Renderer2D *renderer, size_t size, int32_t x,
                           int32_t y, int32_t z) {
  assert(renderer->buffer.vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(renderer->buffer.indecies_count < CM_RENDERER2D_MAX_INDECIES);

  Vertex2D vertecies[] = {
      {{x, y, z}, {0.F, 0.6F, 0.6F, 1.F}},               // a
      {{x + size, y, z}, {0.F, 0.6F, 0.6F, 1.F}},        // b
      {{x + size, y + size, z}, {0.F, 0.6F, 0.6F, 1.F}}, // c
      {{x, y + size, z}, {0.F, 0.6F, 0.6F, 1.F}},        // d
  };

  memcpy(&renderer->buffer.data[renderer->buffer.vertecies_count], vertecies,
         sizeof(vertecies));

  renderer->buffer.vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  renderer->buffer.indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}

void cm_renderer_init_quad_color(Renderer2D *renderer, size_t size, int32_t x,
                                 int32_t y, int32_t z, float r, float g,
                                 float b, float a) {
  assert(renderer->buffer.vertecies_count < CM_RENDERER2D_MAX_VERTECIES);
  assert(renderer->buffer.indecies_count < CM_RENDERER2D_MAX_INDECIES);

  Vertex2D vertecies[] = {
      {{x, y, z}, {r, g, b, a}},               // a
      {{x + size, y, z}, {r, g, b, a}},        // b
      {{x + size, y + size, z}, {r, g, b, a}}, // c
      {{x, y + size, z}, {r, g, b, a}},        // d
  };

  memcpy(&renderer->buffer.data[renderer->buffer.vertecies_count], vertecies,
         sizeof(vertecies));

  renderer->buffer.vertecies_count += CM_RENDERER2D_VERTECIES_PER_QUAD;
  renderer->buffer.indecies_count += CM_RENDERER_INDICES_PER_SQUAD;
}
