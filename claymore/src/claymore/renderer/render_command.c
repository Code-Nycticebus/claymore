#include "render_command.h"

void cm_renderer_draw_indexed(CmRenderBuffer *render_buffer,
                              size_t indices_count) {
  glBindVertexArray(render_buffer->vertex_attribute.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_buffer->index_buffer.id);

  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cm_renderer_clear(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cm_renderer_set_clear_color(vec4s color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
