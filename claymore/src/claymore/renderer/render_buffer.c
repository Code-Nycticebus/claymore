#include "render_buffer.h"

CmVertexBuffer cm_vertex_buffer_create(size_t count, size_t element_size,
                                       const void *data, GLenum draw_mode) {
  CmVertexBuffer vertex_buffer = {
      .id = 0,
      .element_count = count,
      .element_size = element_size,
  };
  glGenBuffers(1, &vertex_buffer.id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.id);
  glBufferData(GL_ARRAY_BUFFER, count * element_size, data, draw_mode);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vertex_buffer;
}

CmVertexAttribute cm_vertex_attribute_create(CmVertexBuffer *vertex_buffer) {
  CmVertexAttribute vertex_attribute = {
      .id = 0,
      .index = 0,
      .buffer = vertex_buffer,
  };
  glGenVertexArrays(1, &vertex_attribute.id);
  return vertex_attribute;
}

void cm_vertex_attribute_push(CmVertexAttribute *attr, size_t count,
                              GLenum type, size_t offset) {
  // Bind vertex_buffer to associate vertex_attribute with it.
  assert(attr->buffer->id);
  glBindBuffer(GL_ARRAY_BUFFER, attr->buffer->id);

  assert(attr->id);
  glBindVertexArray(attr->id);

  glEnableVertexAttribArray(attr->index);
  glVertexAttribPointer(attr->index, count, type, GL_FALSE,
                        attr->buffer->element_size,
                        (const void *)offset); // NOLINT

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  attr->index++;
}

CmIndexBuffer cm_index_buffer_create(CmVertexAttribute *attrib, size_t count,
                                     const uint32_t *indices, GLenum mode) {
  CmIndexBuffer index_buffer = {.id = 0, .count = count, .attrib = attrib};
  // Bind vertex_attrib to associate index buffer with it
  glBindVertexArray(attrib->id);

  glGenBuffers(1, &index_buffer.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               mode);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return index_buffer;
}
