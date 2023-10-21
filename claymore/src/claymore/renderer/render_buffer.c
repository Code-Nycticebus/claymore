#include "render_buffer.h"
#include "render_command.h"

CmVertexArray cm_vertex_array_create(void) {
  CmVertexArray vao = {0};
  glGenVertexArrays(1, &vao.id);
  glBindVertexArray(vao.id);
  return vao;
}
void cm_vertex_array_delete(CmVertexArray *vao) {
  glDeleteVertexArrays(1, &vao->id);
  *vao = (CmVertexArray){0};
}

void cm_vertex_array_push_attrib(CmVertexArray *array, size_t count,
                                 size_t stride, size_t offset) {
  glBindVertexArray(array->id);
  glEnableVertexAttribArray(array->index);
  glVertexAttribPointer(array->index, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset); // NOLINT
  array->index++;
}

void cm_vertex_array_push_attrib_instanced(CmVertexArray *array, size_t count,
                                           size_t stride, size_t offset) {
  glBindVertexArray(array->id);
  glEnableVertexAttribArray(array->index);
  glVertexAttribPointer(array->index, count, GL_FLOAT, GL_FALSE, stride,
                        (void *)offset); // NOLINT
  glVertexAttribDivisor(array->index, 1);
  array->index++;
}

CmIndexBuffer cm_index_buffer_create(const uint32_t *indices, size_t count) {
  CmIndexBuffer ib = {0};
  glGenBuffers(1, &ib.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices,
               GL_STATIC_DRAW);
  ib.count = count;
  return ib;
}

void cm_index_buffer_delete(CmIndexBuffer *ibo) {
  glDeleteBuffers(1, &ibo->id);
  *ibo = (CmIndexBuffer){0};
}

CmVertexBuffer cm_vertex_buffer_create(const void *data, size_t bytes,
                                       CmBufferUsage buffer_usage) {
  CmVertexBuffer vb = {0};
  glGenBuffers(1, &vb.id);
  glBindBuffer(GL_ARRAY_BUFFER, vb.id);
  glBufferData(GL_ARRAY_BUFFER, bytes, data, buffer_usage);
  vb.size = bytes;
  vb.usage = buffer_usage;
  return vb;
}

void cm_vertex_buffer_update(CmVertexBuffer *vbo, const void *data,
                             size_t bytes) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, data);
}

void cm_vertex_buffer_delete(CmVertexBuffer *vbo) {
  glDeleteBuffers(1, &vbo->id);
  *vbo = (CmVertexBuffer){0};
}
